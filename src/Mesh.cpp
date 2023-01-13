#include "Mesh.h"
Mesh::Mesh()
{
	this->vertices.resize(0);
	this->indices.resize(0);

	this->m_VAO = nullptr;
	this->m_VBO = nullptr;
	this->m_IBO = nullptr;
	this->m_shader = nullptr;

	LocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	WorldPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	mat_render = glm::mat4(1.0f);
	mat_Local = glm::mat4(1.0f); 
	mat_transformLocalToWorld = glm::mat4(1.0f); 

}


glm::mat4 Mesh::GetRenderModel() const
{
	return this->mat_render;
}

void Mesh::SetRenderModel(const glm::mat4& _model)
{
	this->mat_render = _model;
}

void Mesh::SetRenderModel(const glm::vec3& _translate)
{
	this->mat_render = glm::translate(glm::mat4(1.0f), _translate);
}

void Mesh::Translate(const glm::vec3& _translate)
{
	WorldPosition += _translate;
	for (auto& v : vertices)
	{
		v.position += _translate;
	}
}

//Mesh::Mesh(std::vector<glm::vec3> _vertices, 
//	//std::vector<glm::vec4> _colorsRGBA,
//	std::vector<glm::vec2> _textureCoordinates, 
//	std::vector<glm::vec3> _normals, 
//	std::vector<unsigned int> _faces)
//{
//	this->vertices = _vertices;
//	//this->colorsRGBA = _colorsRGBA;
//	this->textureCoordinates = _textureCoordinates;
//	this->_normals = normals;
//	this->_faces = faces;
//}
//
//std::vector<glm::vec3> Mesh::GetVertices() const
//{
//	return vertices;
//}
//
////std::vector<glm::vec4> Mesh::GetColors() const
////{
////	return colorsRGBA;
////}
//
//std::vector<glm::vec2> Mesh::GetTextCoordiantes() const
//{
//	return textureCoordinates;
//}
//
//std::vector<glm::vec3> Mesh::GetNormals() const
//{
//	return normals;
//}
//
//std::vector<unsigned int> Mesh::GetFaces() const
//{
//	return faces;
//}

//void Mesh::GenAttriMesh()
//{
//	if (IsEqual() && vertices != nullptr)
//	{
//		for (unsigned int v = 0; v < vertices.size(); ++v)
//		{
//			AttribVertex currentVertex;
//			currentVertex.position  = vertices[v];
//			//currentVertex.colorRGBA = colorsRGBA[v];
//			currentVertex.texCoord = textureCoordinates[v];
//			currentVertex.normals = normals[v];
//
//			attriMesh.emplace_back(currentVertex);
//		}
//
//	}
//}


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
void Mesh::ReadFromObjFile(const char* _filePath, const char* _basePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "res/OBJFiles/Cube.obj", "res/OBJFiles", true);
	if (ret)
	{
		const auto& _vertices = attrib.vertices;
		const auto& _normals = attrib.normals;
		const auto& _texCoords = attrib.texcoords;
		for (unsigned int v = 0; v < _vertices.size() / 3; ++v)
		{
			AttribVertex tmp_v = AttribVertex{
				glm::vec3(_vertices[3 * v], _vertices[3 * v + 1], _vertices[3 * v + 2]),
				glm::vec3(_normals[3 * v], _normals[3 * v + 1], _normals[3 * v + 2]),
				glm::vec2(_texCoords[2 * v], _texCoords[2 * v + 1])
			};
			
			this->vertices.emplace_back(tmp_v);

			LocalPosition += glm::vec3(_vertices[3 * v], _vertices[3 * v + 1], _vertices[3 * v + 2]);
		}
		LocalPosition /= (_vertices.size() / 3);
		WorldPosition = LocalPosition;
	}

	unsigned int index_offset = 0;
	//printf("shape[%ld].num_faces: %lu\n", static_cast<long>(0), static_cast<unsigned long>(shapes[0].mesh.num_face_vertices.size()));

	// For each face
	for (unsigned int f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
		unsigned int fnum = shapes[0].mesh.num_face_vertices[f];

		// For each vertex in the face
		for (unsigned int  v = 0; v < fnum; v++) {
			tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
			//printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
			//	static_cast<long>(v), idx.vertex_index, idx.normal_index,
			//	idx.texcoord_index);
			this->indices.emplace_back(idx.vertex_index);
		}

		index_offset += fnum;
	}
	
	//PrintIndices();
	//PrintVertices();

	PrintCerntralPoint();
}

// What is the mesh filter? https://docs.unity3d.com/Manual/class-MeshFilter.html
void Mesh::BuildMeshFilter()
{
	m_VAO = new VAO();
	m_VAO->Bind();

	m_VBO = new VBO(this->vertices);
	m_IBO = new IBO(this->indices);
	m_VAO->Bind();

	// Linking Vertex Attributes
	m_VAO->LinkAttrib(*m_VBO, 0, 3, GL_FLOAT, sizeof(AttribVertex), (void*)0);
	m_VAO->LinkAttrib(*m_VBO, 1, 3, GL_FLOAT, sizeof(AttribVertex), (void*)(3 * sizeof(float)));
	m_VAO->LinkAttrib(*m_VBO, 2, 2, GL_FLOAT, sizeof(AttribVertex), (void*)(6 * sizeof(float)));
	
	//m_shader = new MyShader(vs, fs);

	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
	//m_shader->Unbind();
}

void Mesh::DrawCallBind()
{
	m_VAO->Bind();
}

void Mesh::DrawCallUnBind()
{
	m_VAO->Unbind();
}

void Mesh::PrintIndices()
{
	std::cout << "Print Indices" << std::endl;
	for (unsigned int f = 0; f < this->indices.size(); f++)
	{
		std::cout << f << " " << this->indices[f] << std::endl;
	}
}

void Mesh::PrintVertices()
{
	std::cout << "Print Vertices" << std::endl;
	unsigned int id = 0;
	for (const auto& v: this->vertices)
	{
		std::cout << id++;
		PrintSingle(v);
		std::cout << std::endl;
	}
}

void Mesh::PrintCerntralPoint()
{
	std::cout << "Print Central Point" << std::endl;	
	std::cout 
		<< "(" << this->LocalPosition.x  << ","
		<< this->LocalPosition.y  << ","
		<< this->LocalPosition.z  << ")"
		<< std::endl;
}

void Mesh::PrintSingle(AttribVertex v)
{
	std::cout << 
		"  v-(" << v.position.x << "," << v.position.y << "," << v.position.z << ") ," <<
		"  n-(" << v.normals.x << "," << v.normals.y << "," << v.normals.z << ") ," << 
		"  t-(" << v.texCoords.x << "," << v.texCoords.y << ")";
}
//bool Mesh::IsEqual()
//{
//	if (vertices.size() /*== colorsRGBA.size()*/ == textureCoordinates.size() == normals.size())
//		return true;
//	return false;
//}
