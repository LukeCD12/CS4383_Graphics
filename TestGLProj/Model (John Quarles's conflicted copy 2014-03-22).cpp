#include "Model.h"
#include <FreeImage.h>

std::map<std::string, GLuint> Model::textureManager;
Model::Model(Shader *shader,  const char* filename, const char* materialPath)
{
	m_shader = shader;
	shininessOverride = 1;
	tinyobj::LoadObj(shapes,filename, materialPath);
	for(int i = 0; i<shapes.size(); i++){
		m_VBO.push_back(0);
		m_NBO.push_back(0);
		m_IBO.push_back(0);
		m_TCBO.push_back(0);
		textures.push_back(struct Textures());
		glGenBuffers(1, &m_VBO[i]); // generate a 'name' for the VBO
		glGenBuffers(1, &m_NBO[i]); // generate a 'name' for the NBO
		glGenBuffers(1, &m_IBO[i]); // generate a 'name' for the IBO
		if(shapes[i].material.ambient_texname.length() == 0 && shapes[i].material.diffuse_texname.length() == 0 && shapes[i].material.specular_texname.length() == 0 && shapes[i].material.normal_texname.length() == 0){
			m_TCBO[i] = -1;
		}else{
			glEnable(GL_TEXTURE_2D);
			GLuint id = textureManager[shapes[i].material.diffuse_texname];
			if(id==0){
				glGenBuffers(1, &m_TCBO[i]);
				if(shapes[i].material.diffuse_texname.length()!=0){
					std::string pathandname = materialPath;
					pathandname+=shapes[i].material.diffuse_texname;			
					glActiveTexture(GL_TEXTURE0);
					//need to save the IDs for each shape
					textures[i].diffuse = LoadTexture(pathandname.c_str());
					textureManager[shapes[i].material.diffuse_texname] = textures[i].diffuse;
				}
			}else
			{
				if(shapes[i].material.diffuse_texname.length()!=0){
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, id);
					textures[i].diffuse = id;
				}
			}

		}

				
		
	}
	
	updateBuffers();
	
}


GLuint Model::LoadTexture(const char* filename, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	/*//if this texture ID is in use, unload the current texture
	if(m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));
		*/

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
//	m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return ID
	return gl_texID;
}

void  Model::setOverrideDiffuseMaterial(glm::vec4 color){diffuseOverride = glm::vec4(color);}
void  Model::setOverrideSpecularMaterial(glm::vec4 color){specularOverride = glm::vec4(color);}
void  Model::setOverrideSpecularShininessMaterial(float shine){shininessOverride = shine;}
void  Model::setOverrideAmbientMaterial(glm::vec4 color){ambientOverride = glm::vec4(color);}
void  Model::setOverrideEmissiveMaterial(glm::vec4 color){emissiveOverride = glm::vec4(color);}

/*
Render the mesh to the screen
@param Modelview - the model view matrix that defines where the camera is looking
@param Projection - the projection matrix that defines how 3D vertices are projected on the 2D screen.
*/
void Model::render(glm::mat4 ModelView, glm::mat4 Projection, bool useObjMaterial) {

	m_shader->Activate(); // Bind shader.
	
	//update the variables in the shader program
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader


	//m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader
	for(int i = 0; i < shapes.size(); i++){
		if(useObjMaterial){
			m_shader->SetUniform("surfaceDiffuse",  glm::vec4(shapes[i].material.diffuse[0], shapes[i].material.diffuse[1], shapes[i].material.diffuse[2], 1.0));
			m_shader->SetUniform("surfaceSpecular", glm::vec4(shapes[i].material.specular[0], shapes[i].material.specular[1], shapes[i].material.specular[2], 1.0));
			m_shader->SetUniform("surfaceAmbient", glm::vec4(shapes[i].material.ambient[0], shapes[i].material.ambient[1], shapes[i].material.ambient[2], 1.0));
			m_shader->SetUniform("shininess", shapes[i].material.shininess);
			m_shader->SetUniform("surfaceEmissive", glm::vec4(shapes[i].material.emission[0], shapes[i].material.emission[1], shapes[i].material.emission[2], 1.0));
		}else{
			m_shader->SetUniform("surfaceDiffuse", diffuseOverride);
			m_shader->SetUniform("surfaceSpecular", specularOverride);
			m_shader->SetUniform("surfaceAmbient", ambientOverride);
			m_shader->SetUniform("shininess", shininessOverride);
			m_shader->SetUniform("surfaceEmissive", emissiveOverride);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]); // Bind VBO.
		glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
		glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0); // Attribute pointer.
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]); // Bind NBO.
		glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
		glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
		if(m_TCBO[i] !=-1){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[i].diffuse);
			m_shader->SetUniform("diffuseTexture", 0);
			m_shader->SetUniform("useTexture", 1);
			glBindBuffer(GL_ARRAY_BUFFER, m_TCBO[i]); // Bind TCBO.
			glEnableVertexAttribArray((*m_shader)["vertexTextureCoordinates"]); // Enable normal attribute.
			glVertexAttribPointer((*m_shader)["vertexTextureCoordinates"], 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
		}
		else{
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableVertexAttribArray((*m_shader)["vertexTextureCoordinates"]);
			m_shader->SetUniform("useTexture", 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]); // Bind IBO.
		glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, 0); // Draw using indices
	}
	m_shader->DeActivate(); // Unbind shader.
}



void Model::updateBuffers()
{
	for(int i =0; i<shapes.size();i++){
		
		// Tell OpenGL which VBO you want to work on right now
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
		// Fillthe VBO with vertex data.
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), &shapes[i].mesh.positions[0], GL_STATIC_DRAW);
		// Tell OpenGL which VBO you want to work on right now
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]);
		// Fillthe VBO with vertex data.
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size() * sizeof(float), &shapes[i].mesh.normals[0], GL_STATIC_DRAW);

		//fill the buffer with texture coordinates
		if(m_TCBO[i]!=-1){
			glBindBuffer(GL_ARRAY_BUFFER, m_TCBO[i]);
			glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.texcoords.size() * sizeof(float), &shapes[i].mesh.texcoords[0], GL_STATIC_DRAW);
		}
		// Bind ibo to the index buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]);
		// Fill index buffer with index data.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), &shapes[i].mesh.indices[0], GL_STATIC_DRAW);
		
		
	}

}

