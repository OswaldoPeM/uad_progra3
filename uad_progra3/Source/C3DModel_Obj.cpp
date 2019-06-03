#include "../stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "../Include/C3DModel.h"
#include "../Include/C3DModel_Obj.h"

/* 
*/
C3DModel_Obj::C3DModel_Obj() :
	C3DModel(),
	m_currentVertex(0),
	m_currentNormal(0),
	m_currentUV(0),
	m_currentFace(0)
{
	cout << "Constructor: C3DModel_Obj()" << endl;
}

/* 
*/
C3DModel_Obj::~C3DModel_Obj()
{
	cout << "Destructor: C3DModel_Obj()" << endl;
	reset();
}

/*
*/
void C3DModel_Obj::reset()
{
	C3DModel::reset();

	m_currentVertex = 0;
	m_currentNormal = 0;
	m_currentUV = 0;
	m_currentFace = 0;
}

/*
*/
bool C3DModel_Obj::loadFromFile(const char * const filename)
{
	bool readFileOk = false;

	// Free any previous resources
	reset();

	// First pass is to count the number of vertices, normals, UVs, faces
	readFileOk = readObjFile(filename, true);

	// Display count
	cout << "Finished reading 3D model" << endl;
	cout << "Vertices: " << m_numVertices << endl;
	cout << "Normals: " << m_numNormals << endl;
	cout << "UVCoords: " << m_numUVCoords << endl;
	cout << "Faces: " << m_numFaces << endl;

	if (readFileOk)
	{
		// Check for MAX number of faces
		if (m_numVertices >= 65535 || m_numNormals >= 65535 || m_numUVCoords >= 65535)
		{
			cout << "Error: object cannot have more than 65535 vertices" << endl;
			cout << "Object attempted to load has: " << m_numVertices << " vertices" << endl;
			return false;
		}

		// If model was read without normals or UVCoords, we'll set a default value for them
		// i.e.:
		//   0,0 for UV coords
		//   face normal for normal
		if (m_numNormals == 0)
		{
			m_modelHasNormals = false;
			m_numNormals = m_numVertices;
		}
		else
		{
			m_modelHasNormals = true;
		}

		if (m_numUVCoords == 0)
		{
			m_numUVCoords = m_numVertices;
			m_modelHasUVs = false;
		}
		else
		{
			m_modelHasUVs = true;
		}

		// Allocate memory for the arrays

		// C3DModel variables
		m_verticesRaw = new float[m_numVertices * 3];
		m_normalsRaw = new float[m_numNormals * 3];
		m_uvCoordsRaw = new float[m_numUVCoords * 2];
		m_vertexIndices = new unsigned short[m_numFaces * 3];
		m_normalIndices = new unsigned short[m_numFaces * 3];
		m_UVindices = new unsigned short[m_numFaces * 3];
		m_materials[m_actualMaterial].scope.push_back(m_numFaces);

		// Zero-out indices arrays
		memset(m_vertexIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_normalIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_UVindices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_verticesRaw, 0, sizeof(float) * m_numVertices * 3);
		memset(m_normalsRaw, 0, sizeof(float) * m_numNormals * 3);
		memset(m_uvCoordsRaw, 0, sizeof(float) * m_numUVCoords * 2);

		// Second pass is to read the data
		readFileOk = readObjFile(filename, false);

		if (readFileOk)
		{
			m_Initialized = true;

			if (!m_modelHasNormals)
			{
				computeFaceNormals();
			}
		}
	}
	else
	{
		cout << "Error ocurred while reading 3D model." << endl;
	}

	return readFileOk;
}

/*
*/
bool C3DModel_Obj::readObjFile(const char * filename, bool countOnly)
{
	ifstream infile;
	string lineBuffer;
	bool readFileOk = true;
	int lineNumber = 0;

	infile.open(filename);

	while (!infile.eof())
	{
		getline(infile, lineBuffer);
		lineNumber++;

		if (!(this->parseObjLine(lineBuffer, countOnly, lineNumber)))
		{
			readFileOk = false;
			break;
		}
		// cout << lineBuffer << endl;
	}

	infile.close();

	return readFileOk;
}

/*
 * NOTE: This code reads the .obj file format and can skip normal/UV coords information if the file doesn't have it,
 *
 * TO-DO...
 * Also, this reads files with triangles, not quads. This is also a TO-DO...
 */
bool C3DModel_Obj::parseObjLine(std::string line, bool countOnly, int lineNumber)
{
	bool parsed = false;
	bool unrecognizedLine = false;

	bool readingVertex = false;
	bool readingNormal = false;
	bool readingUV = false;
	bool readingFace = false;
	bool readingTexture = false;
	bool readingUseMlt = false;
	bool isQuad = false;

	char *nextToken = NULL;
	char *token = NULL;

	const char *delimiterToken = " \t";
	const char *delimiterFace = "/";

	int currentToken = 0;
	int numTokens = 0;
	int numExpectedTokens = 4;

	std::vector<std::string> tokens;

	std::string materialName;
	std::string materialFilename;

	token = strtok_s((char *)line.c_str(), delimiterToken, &nextToken);

	// Line without space or tab separators (most likely a null string)
	if (token == NULL)
	{
		parsed = true;
	}

	// If there are any tokens left
	while (token != NULL)
	{
		// If first token
		if (currentToken == 0)
		{
			// Vertex
			if (0 == strcmp(token, "v"))
			{
				readingVertex = true;

				if (countOnly)
				{
					m_numVertices++;
				}
			}
			// Normal
			else if (0 == strcmp(token, "vn"))
			{
				readingNormal = true;

				if (countOnly)
				{
					m_numNormals++;
				}
			}
			// Vertex texture
			else if (0 == strcmp(token, "vt"))
			{
				readingUV = true;

				if (countOnly)
				{
					m_numUVCoords++;
				}
			}
			// Face
			else if (0 == strcmp(token, "f"))
			{
				readingFace = true;

				if (countOnly)
				{
					// Check if this line is a quad or a triangle
					std::string nextStrToken(nextToken);

					m_numFaces++;
				}
			}
			// Texture
			else if (0 == strcmp(token, "mtllib"))
			{
				readingTexture = true;
			}
			// Material
			else if (0 == strcmp(token, "usemtl")) {
				readingUseMlt = true;
				if (countOnly)
				{
					std::string nextStrToken(nextToken);
					// asign the scope for each material
					for (int i = 0; i <= m_materials.size(); i++)
					{
						if (i == m_materials.size())
						{
							Material mat;
							if (m_materials.size() > 0) 
							{
								m_materials[m_actualMaterial].scope.push_back(m_numFaces); 
							}
							mat.name.append(nextStrToken);
							mat.scope.push_back(m_numFaces);
							m_materials.push_back(mat);
							m_actualMaterial = i;
							/*if (i > 0) { m_materials[i].scope[0] += 1; }*/
							break;
						}
						else if (m_materials[i].name == nextStrToken)
						{
							if (m_actualMaterial == i)break;
							m_materials[m_actualMaterial].scope.push_back(m_numFaces);
							m_materials[i].scope.push_back(m_numFaces);
							m_actualMaterial = i;
							break;
						}
					}
				}
			}
			else
			{
				// Unrecognized line
				if (countOnly)
				{
					cout << "Ignoring line #" << lineNumber << ": " << line << endl;
				}
				unrecognizedLine = true;
			}

			// If this line has an unrecognized format
			// OR If we're in count only mode and we processed the first token, 
			// break to exit method and go to next line
			if (countOnly || unrecognizedLine)
			{
				return true;
			}
		}
		else
		{
			// Add token to vector
			tokens.push_back(std::string(token));
		}

		// Read next token
		token = strtok_s(NULL, delimiterToken, &nextToken);
		currentToken++;

		// No more tokens
		if (token == NULL)
		{
			//  If we're NOT in "count only" mode
			if (!countOnly)
			{
				// Verify we have the expected number of tokens
				if (currentToken != numExpectedTokens && !readingTexture && !readingUseMlt)
				{
					cout << "Ignoring line, number of tokens doesn't match the expected." << endl;
					cout << line.c_str() << endl;
				}
				else
				{
					// Create the vertex|normal|UVcoord|face
					if (readingVertex || readingNormal || readingUV)
					{
						// Add the 3 effective tokens to the appropriate array
						float values[3];
						values[0] = (float)atof(tokens[0].c_str());
						values[1] = (float)atof(tokens[1].c_str());
						values[2] = (float)atof(tokens[2].c_str());
						parsed = true;

						if (readingVertex && m_verticesRaw != NULL &&
							(m_currentVertex < m_numVertices) && m_numVertices > 0)
						{
							m_verticesRaw[(m_currentVertex * 3)] = values[0];
							m_verticesRaw[(m_currentVertex * 3) + 1] = values[1];
							m_verticesRaw[(m_currentVertex * 3) + 2] = values[2];

							m_currentVertex++;
						}
						else if (readingNormal && m_normalsRaw != NULL &&
							(m_currentNormal < m_numNormals) && m_numNormals > 0)
						{
							m_normalsRaw[(m_currentNormal * 3)] = values[0];
							m_normalsRaw[(m_currentNormal * 3) + 1] = values[1];
							m_normalsRaw[(m_currentNormal * 3) + 2] = values[2];

							m_currentNormal++;
						}
						else if (readingUV && m_uvCoordsRaw != NULL &&
							(m_currentUV < m_numUVCoords) && m_numUVCoords > 0)
						{
							m_uvCoordsRaw[(m_currentUV * 2)] = values[0];
							m_uvCoordsRaw[(m_currentUV * 2) + 1] = values[1];

							m_currentUV++;
						}
						else
						{
							parsed = false;
						}
					}
					else if (
						readingFace
						&& m_vertexIndices != NULL
						&& m_normalIndices != NULL
						&& m_UVindices != NULL
						&& (m_currentFace < (m_numFaces * 3))
						&& m_numFaces > 0
						)
					{
						// Parse the current token to extract the vertex|normal|UV index
						parsed = true;
						nextToken = NULL;
						token = NULL;

						// Loop through all 3 groups of indices, i.e:    1/3/4    3/5/2    2/3/1
						//
						// token[0] = 1/3/4
						// token[1] = 3/5/2
						// token[2] = 2/3/1
						for (int i = 0; i < 3 && i < tokens.size(); i++)
						{
							currentToken = -1;

							// Get group of indices and split it into tokens with '/' as delimiter
							token = strtok_s((char *)tokens[i].c_str(), delimiterFace, &nextToken);

							while (nextToken != NULL && *nextToken != '\0')
							{
								currentToken++;

								if (token != NULL)
								{
									switch (currentToken)
									{
									case 0:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_vertexIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 1:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_UVindices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 2:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_normalIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									}
								}

								token = strtok_s(NULL, delimiterFace, &nextToken);

								// Last token
								if (token != NULL &&
									(nextToken == NULL || (nextToken != NULL && *nextToken == '\0'))
									)
								{
									currentToken++;

									switch (currentToken)
									{
									case 0:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_vertexIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 1:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_UVindices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 2:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_normalIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									}
								}
							} // while (nextToken != NULL)
						} // for

						m_currentFace += 3;
						token = NULL;
						nextToken = NULL;
					} // reading face
					else if (readingTexture)
					{
						if (readMtllib(tokens[0], materialName, materialFilename))
						{
							m_modelTextureFilename = new char[materialFilename.size() + 1];
							memset(m_modelTextureFilename, 0, materialFilename.size() + 1);
							memcpy(m_modelTextureFilename, materialFilename.c_str(), materialFilename.size());
							m_modelHasTextures = true;
							parsed = true;
						}
						else
						{
							parsed = false;
						}
					}
					//reading MLT
					else if (readingUseMlt)
					{
						parsed = true;
					}
				} // reading
			} // !count only
		} // No more tokens
	}

	return parsed;
}

/* 
 */
bool C3DModel_Obj::readMtllib(std::string mtlLibFilename, std::string &materialName, std::string &materialFilename)
{
	bool readTextureName = false;

	ifstream infile;
	string lineBuffer;
	char *nextToken = nullptr;
	char *token = nullptr;
	const char *delimiterToken = " \t";
	bool readingMaterialName = false;
	bool readingMaterialFilename = false;
	int numToken = 0;

	materialName.clear();
	materialFilename.clear();

	infile.open(mtlLibFilename);

	while (!infile.eof())
	{
		getline(infile, lineBuffer);
		
		readingMaterialName = false;
		readingMaterialFilename = false;
		numToken = 0;
		token = strtok_s((char *)lineBuffer.c_str(), delimiterToken, &nextToken);
		++numToken;

		// If there are any tokens left
		while (token != nullptr)
		{
			if (numToken == 1)
			{
				if (0 == strcmp(token, "newmtl"))
				{
					readingMaterialName = true;
				}
				else if (0 == strcmp(token, "map_Kd"))
				{
					readingMaterialFilename = true;
				}
			}
			else if (numToken == 2)
			{
				if (readingMaterialName)
				{
					for (int i = 0; i < m_materials.size(); i++)
					{
						if (m_materials[i].name == token)m_actualMaterial = i;
					}
				}
				else if (readingMaterialFilename)
				{
					materialFilename.append(token);
					m_materials[m_actualMaterial].fileName = new char[materialFilename.size() + 1];
					memset(m_materials[m_actualMaterial].fileName, 0, materialFilename.size() + 1);
					memcpy(m_materials[m_actualMaterial].fileName, materialFilename.c_str(), materialFilename.size());
					materialFilename.clear();
					readTextureName = true;
					break;
				}
			}

			token = strtok_s(nullptr, delimiterToken, &nextToken);
			++numToken;
		}

		// For now, only read the first material
		if (readTextureName)
		{
			//break;
		}
	}
	materialName.append(m_materials[0].name);
	materialFilename.append(m_materials[0].fileName);
	m_numMaterials = m_materials.size();
	infile.close();
	if (m_materials.size()<1)
	{
		return false;
	}
	else
	{
		return true;
	}
}