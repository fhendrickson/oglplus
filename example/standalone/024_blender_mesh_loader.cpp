/**
 *  @example standalone/024_blender_mesh_loader.cpp
 *  @brief Shows the usage of the .blend file parser utility
 *
 *  Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/imports/blend_file.hpp>

#include <vector>
#include <fstream>

class BlenderMeshExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;

	oglplus::Program prog;

	oglplus::LazyUniform<oglplus::Mat4f> camera_matrix;
	oglplus::LazyUniform<GLint> face_normals;

	oglplus::VertexArray meshes;
	oglplus::Buffer positions, normals, indices;

	GLuint element_count;

	oglplus::Vec3f camera_target;
	float camera_distance;
public:
	BlenderMeshExample(int argc, const char* argv[])
	 : prog()
	 , camera_matrix(prog, "CameraMatrix")
	 , face_normals(prog, "FaceNormals")
	 , element_count(0)
	{
		using namespace oglplus;

		VertexShader vs;
		vs.Source(
			"#version 330\n"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 LightPos;"

			"mat4 Matrix = ProjectionMatrix * CameraMatrix;"

			"in vec3 Position;"
			"in vec3 Normal;"

			"out vec3 vertNormal;"
			"out vec3 vertLightDir;"

			"void main(void)"
			"{"
			"	vertNormal = Normal;"
			"	vertLightDir = LightPos - Position;"
			"	gl_Position = Matrix * vec4(Position, 1.0);"
			"}"
		);
		vs.Compile();
		prog.AttachShader(vs);

		GeometryShader gs;
		gs.Source(
			"#version 330\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices=3) out;"

			"uniform bool FaceNormals;"

			"in vec3 vertNormal[3];"
			"in vec3 vertLightDir[3];"

			"out vec3 geomNormal;"
			"out vec3 geomLightDir;"

			"void main(void)"
			"{"
			"	vec3 fn;"
			"	if(FaceNormals)"
			"	{"
			"		vec3 p0 = gl_in[0].gl_Position.xyz;"
			"		vec3 p1 = gl_in[1].gl_Position.xyz;"
			"		vec3 p2 = gl_in[2].gl_Position.xyz;"
			"		fn = normalize(cross(p1-p0, p2-p0));"
			"	}"

			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		gl_Position = gl_in[v].gl_Position;"
			"		if(FaceNormals) geomNormal = fn;"
			"		else geomNormal = vertNormal[v];"
			"		geomLightDir = vertLightDir[v];"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();
		prog.AttachShader(gs);

		FragmentShader fs;
		fs.Source(
			"#version 330\n"

			"in vec3 geomNormal;"
			"in vec3 geomLightDir;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"	vec3 MatColor = vec3(0.5, 0.5, 0.5);"

			"	float Ambient = 0.3;"
			"	float Diffuse = max(dot("
			"		normalize(geomNormal),"
			"		normalize(geomLightDir)"
			"	)+0.1, 0.0);"

			"	fragColor = MatColor * LightColor * (Diffuse + Ambient);"
			"}"
		);
		fs.Compile();
		prog.AttachShader(fs);

		prog.Link();
		prog.Use();

		gl.PrimitiveRestartIndex(0);
		// vectors with vertex position and normals
		// the values at index 0 is unused
		// 0 is used as primitive restart index
		std::vector<GLfloat> pos_data(3, 0.0);
		std::vector<GLfloat> nml_data(3, 0.0);
		// index offset starting at 1
		GLuint index_offset = 1;
		// vectors with vertex indices
		std::vector<GLuint> idx_data(1, 0);

		// open an input stream
		std::ifstream input(argc>1? argv[1]: "./test.blend");
		// check if we succeeded
		if(!input.good())
			throw std::runtime_error("Error opening file for reading");
		// parse the input stream
		imports::BlendFile blend_file(input);
		// get the file's global block
		auto glob_block = blend_file.StructuredGlobalBlock();

		// get the default scene
		auto scene_block = blend_file[glob_block.curscene];
		//
		// get the pointer to the first object in the scene
		auto object_link_ptr = scene_block.Field<void*>("base.first").Get();
		// and go through the whole list of objects
		while(object_link_ptr)
		{
			// for each list element open the linked list block
			auto object_link_block = blend_file[object_link_ptr];
			// get the pointer to its object
			auto object_ptr = object_link_block.Field<void*>("object").Get();
			// open the object block (if any)
			if(object_ptr)
			{
				auto object_block = blend_file[object_ptr];
				// get the data pointer
				auto object_data_ptr = object_block.Field<void*>("data").Get();
				// open the data block (if any)
				if(object_data_ptr)
				{
					auto object_data_block = blend_file[object_data_ptr];
					// if it is a mesh
					if(object_data_block.StructureName() == "Mesh")
					{
						std::size_t n_verts = 0;
						// get the vertex block pointer
						auto vertex_ptr = object_data_block.Field<void*>("mvert").Get();
						// open the vertex block (if any)
						if(vertex_ptr)
						{
							auto vertex_block = blend_file[vertex_ptr];
							// get the number of vertices in the block
							n_verts = vertex_block.BlockElementCount();
							// get the vertex coordinate and normal fields
							auto vertex_co_field = vertex_block.Field<float>("co");
							auto vertex_no_field = vertex_block.Field<short>("no");
							// make two vectors of position and normal data
							std::vector<GLfloat> ps(3 * n_verts);
							std::vector<GLfloat> ns(3 * n_verts);
							for(std::size_t v=0; v!=n_verts; ++v)
							{
								// (transpose y and z axes)
								// get the positional coordinates
								ps[3*v+0] = vertex_co_field.Get(v, 0);
								ps[3*v+1] = vertex_co_field.Get(v, 2);
								ps[3*v+2] = vertex_co_field.Get(v, 1);
								// get the normals
								ns[3*v+0] = vertex_no_field.Get(v, 0);
								ns[3*v+1] = vertex_no_field.Get(v, 2);
								ns[3*v+2] = vertex_no_field.Get(v, 1);
							}
							// append the values
							pos_data.insert(pos_data.end(), ps.begin(), ps.end());
							nml_data.insert(nml_data.end(), ns.begin(), ns.end());
						}

						// get the face block pointer
						auto face_ptr = object_data_block.Field<void*>("mface").Get();
						// open the face block (if any)
						if(face_ptr)
						{
							auto face_block = blend_file[face_ptr];
							// get the number of faces in the block
							std::size_t n_faces = face_block.BlockElementCount();
							// get the vertex index fields of the face
							auto face_v1_field = face_block.Field<int>("v1");
							auto face_v2_field = face_block.Field<int>("v2");
							auto face_v3_field = face_block.Field<int>("v3");
							auto face_v4_field = face_block.Field<int>("v4");
							// make a vector of index data
							std::vector<GLuint> is(5 * n_faces);
							for(std::size_t f=0; f!=n_faces; ++f)
							{
								// get face vertex indices
								int v1 = face_v1_field.Get(f);
								int v2 = face_v2_field.Get(f);
								int v3 = face_v3_field.Get(f);
								int v4 = face_v4_field.Get(f);

								is[5*f+0] = v1+index_offset;
								is[5*f+1] = v2+index_offset;
								is[5*f+2] = v3+index_offset;
								is[5*f+3] = v4?v4+index_offset:0;
								is[5*f+4] = 0; // primitive restart index
							}
							// append the values
							idx_data.insert(idx_data.end(), is.begin(), is.end());
						}

						// get the poly block pointer
						auto poly_ptr = object_data_block.TryGet<void*>("mpoly", nullptr);
						// and the loop block pointer
						auto loop_ptr = object_data_block.TryGet<void*>("mloop", nullptr);
						// open the poly and loop blocks (if we have both)
						if(poly_ptr && loop_ptr)
						{
							auto poly_block = blend_file[poly_ptr];
							auto loop_block = blend_file[loop_ptr];
							// get the number of polys in the block
							std::size_t n_polys = poly_block.BlockElementCount();
							// get the fields of poly and loop
							auto poly_loopstart_field = poly_block.Field<int>("loopstart");
							auto poly_totloop_field = poly_block.Field<int>("totloop");
							auto loop_v_field = loop_block.Field<int>("v");

							// make a vector of index data
							std::vector<GLuint> is;
							for(std::size_t f=0; f!=n_polys; ++f)
							{
								int ls = poly_loopstart_field.Get(f);
								int tl = poly_totloop_field.Get(f);

								for(int l=0; l!=tl; ++l)
								{
									int v = loop_v_field.Get(ls+l);
									is.push_back(v+index_offset);
								}
								is.push_back(0); // primitive restart index
							}
							// append the values
							idx_data.insert(idx_data.end(), is.begin(), is.end());
						}
						index_offset += n_verts;
					}
				}
			}
			// and get the pointer to the nex block
			object_link_ptr = object_link_block.Field<void*>("next").Get();
		}

		meshes.Bind();

		positions.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, pos_data);
			VertexAttribArray attr(prog, "Position");
			attr.Setup(3, DataType::Float);
			attr.Enable();
		}

		normals.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, nml_data);
			VertexAttribArray attr(prog, "Normal");
			attr.Setup(3, DataType::Float);
			attr.Enable();
		}

		indices.Bind(Buffer::Target::ElementArray);
		Buffer::Data(Buffer::Target::ElementArray, idx_data);

		element_count = idx_data.size();

		GLfloat min_x = pos_data[3], max_x = pos_data[3];
		GLfloat min_y = pos_data[4], max_y = pos_data[4];
		GLfloat min_z = pos_data[5], max_z = pos_data[5];
		for(std::size_t v=1, vn=pos_data.size()/3; v!=vn; ++v)
		{
			GLfloat x = pos_data[v*3+0];
			GLfloat y = pos_data[v*3+1];
			GLfloat z = pos_data[v*3+2];

			if(min_x > x) min_x = x;
			if(min_y > y) min_y = y;
			if(min_z > z) min_z = z;
			if(max_x < x) max_x = x;
			if(max_y < y) max_y = y;
			if(max_z < z) max_z = z;
		}

		camera_target = Vec3f(
			(min_x + max_x) * 0.5,
			(min_y + max_y) * 0.5,
			(min_z + max_z) * 0.5
		);
		camera_distance = 1.1*Distance(camera_target, Vec3f(min_x, min_y, min_z))+1.0;


		gl.ClearColor(0.17f, 0.22f, 0.17f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::PrimitiveRestart);

		Uniform<Vec3f>(prog, "LightPos").Set(
			Vec3f(7.0, 3.0, -1.0)
		);
	}

	void Reshape(void)
	{
		using namespace oglplus;

		gl.Viewport(Width(), Height());

		Uniform<Mat4f>(prog, "ProjectionMatrix").Set(
			CamMatrixf::PerspectiveX(
				Degrees(48),
				Width()/Height(),
				1, 100
			)
		);
	}

	void Render(void)
	{
		using namespace oglplus;

		double t = FrameTime();

		gl.Clear().ColorBuffer().DepthBuffer();
		//
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				camera_target,
				camera_distance,
				-FullCircles(t / 17.0),
				Degrees(SineWave(t / 21.0) * 35)
			)
		);

		face_normals.Set(int(t*0.25) % 2);

		gl.DrawElements(
			PrimitiveType::TriangleFan,
			element_count,
			DataType::UnsignedInt
		);
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<BlenderMeshExample>(
		"Example of the imports::BlendFile utility usage",
		argc, argv
	);
}
