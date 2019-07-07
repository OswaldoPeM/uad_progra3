#include "CGrid.h"



void CGrid::reset()
{
	m_row = 0;
	m_col = 0;
	if (m_grid != nullptr) {
		for (int i = 0; i < m_col; i++)
		{
			delete[] m_grid[i];
		}
		delete[] m_grid;
		m_grid = nullptr;
	}
	if (vData != nullptr) {
		delete[] vData;
		vData = nullptr;
	}
	if (vertexUVs != nullptr) {
		delete[] vertexUVs;
		vertexUVs = nullptr;
	}
}

void CGrid::setTIndicesSize()
{
	tIndices = new unsigned short[m_numFacesGrid * 3];
	m_numIndicesVert = m_numFacesGrid;
}

void CGrid::setNDataSize()
{
	nData = new float[m_numFacesGrid * 3];
	for (int i = 0; i < m_numFacesGrid * 3; i++)
	{
		nData[i] = 0.0f;
	}
	m_numNormals = m_numFacesGrid;
}

void CGrid::setNIndices()
{
	nIndices = new unsigned short[m_numFacesGrid * 3];
	for (int i = 0; i < m_numFacesGrid; i++)
	{
		nIndices[(i * 3)] = i;
		nIndices[(i * 3)+1] = i;
		nIndices[(i * 3)+2] = i;
	}
	m_numIndicesNormal = m_numFacesGrid;
}

void CGrid::setVDataSize()
{
	vData = new float[m_col*m_row * 3 * 6];
	m_numVertices = m_col * m_row * 6;
}

void CGrid::setVertexUVsSize()
{
	vertexUVs = new float[m_col*m_row * 6];
	m_nummUVCoord = m_col * m_row * 6;
}

void CGrid::addVData(int &index, CVector3 *vertex)
{
	vData[index++] = vertex[0].getX();
	vData[index++] = vertex[0].getY();
	vData[index++] = vertex[0].getZ();
	vData[index++] = vertex[1].getX();
	vData[index++] = vertex[1].getY();
	vData[index++] = vertex[1].getZ();
	vData[index++] = vertex[2].getX();
	vData[index++] = vertex[2].getY();
	vData[index++] = vertex[2].getZ();
	vData[index++] = vertex[3].getX();
	vData[index++] = vertex[3].getY();
	vData[index++] = vertex[3].getZ();
	vData[index++] = vertex[4].getX();
	vData[index++] = vertex[4].getY();
	vData[index++] = vertex[4].getZ();
	vData[index++] = vertex[5].getX();
	vData[index++] = vertex[5].getY();
	vData[index++] = vertex[5].getZ();

}

void CGrid::addVertexUVs()
{
	int length = m_col * m_row * 6;
	for (int i = 0; i < length; i++)
	{
		vertexUVs[i++] = 0.5f;
		vertexUVs[i] = .90f;
	}
}

void CGrid::addTInices(int i, int j, int &index)
{
	tIndices[index++] = (m_col * 6 * i) + (j * 6);
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 1;

	tIndices[index++] = (m_col * 6 * i) + (j * 6);
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;

	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 3;

	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 4;
}

void CGrid::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

void CGrid::normalize(float v[3])
{
	float d = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (d == 0.0f)
	{
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}


CGrid::CGrid()
{
}


CGrid::~CGrid()
{
}

void CGrid::initialize(int cols, int  rows, float size, bool flat)
{
	reset();
	//Setters
	int vDataIndx = 0, tIndIndex = 0;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];
	m_row = rows;
	m_col = cols;
	setVDataSize();
	setVertexUVsSize();
	m_numFacesGrid = m_col * m_row * 4;
	setTIndicesSize();
	setNIndices();
	//create a bidimiensional array of CGridCells
	m_grid = new CGridCell*[m_row];
	for (int i = 0; i < m_row; i++)
	{
		m_grid[i] = new CGridCell[m_col];
	}

	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (i == 0)if (j == 0)m_grid[i][j] = CGridCell(nullptr, size, flat);
			m_grid[i][j] = CGridCell(m_grid[0][0].getVecVerx(), nullptr, flat, i, j);

			addVData(vDataIndx, m_grid[i][j].getVecVerx());
			addTInices(i, j, tIndIndex);
		}
	}
	addVertexUVs();

	for (int i = 0; i < m_numFacesGrid; i++)
	{
		// Vertex 1
		v1[0] = vData[tIndices[i * 3]];
		v1[1] = vData[tIndices[i * 3] + 1];
		v1[2] = vData[tIndices[i * 3] + 2];

		// Vertex 2
		v2[0] = vData[tIndices[(i * 3) + 1]];
		v2[1] = vData[tIndices[(i * 3) + 1] + 1];
		v2[2] = vData[tIndices[(i * 3) + 1] + 2];

		// Vertex 3
		v3[0] = vData[tIndices[(i * 3) + 2]];
		v3[1] = vData[tIndices[(i * 3) + 2] + 1];
		v3[2] = vData[tIndices[(i * 3) + 2] + 2];

		// Vector from v2 to v1
		v1v2[0] = v1[0] - v2[0];
		v1v2[1] = v1[1] - v2[1];
		v1v2[2] = v1[2] - v2[2];

		// Vector from v2 to v3
		v1v3[0] = v3[0] - v2[0];
		v1v3[1] = v3[1] - v2[1];
		v1v3[2] = v3[2] - v2[2];

		normcrossprod(v1v2, v1v3, norm);

		nData[i * 3] = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];
	}
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_gridShaderPrgmID,
		&m_graphicsMemoriObjectId,
		vData,
        m_numVertices,
		nData,
		m_numNormals,
		vertexUVs,
		5,
		tIndices,
		6,
		nIndices,
		6,
		tIndices,
		6
	);
}

CVector3 CGrid::getPos(int x, int y)
{
	return m_grid[x][y].getPos();
}
