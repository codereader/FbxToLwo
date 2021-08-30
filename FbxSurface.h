#pragma once

#include <vector>
#include <unordered_map>
#include "export/ArbitraryMeshVertex.h"
#include "export/VertexHashing.h"

namespace model
{

class FbxSurface
{
public:
	std::vector<unsigned int> indices;
	std::vector<ArbitraryMeshVertex> vertices;
	std::string material;

	// Hash index to share vertices with the same set of attributes
	std::unordered_map<ArbitraryMeshVertex, std::size_t> vertexIndices;

	const std::vector<ArbitraryMeshVertex>& getVertexArray() const
	{
		return vertices;
	}

	const std::vector<unsigned int>& getIndexArray() const
	{
		return indices;
	}

	const std::string& getActiveMaterial() const
	{
		return material;
	}

	void addVertex(const ArbitraryMeshVertex& vertex)
	{
		// Try to look up an existing vertex or add a new index
		auto emplaceResult = vertexIndices.try_emplace(vertex, vertices.size());

		if (emplaceResult.second)
		{
			// This was a new vertex, copy it to the vertex array
			vertices.emplace_back(emplaceResult.first->first);
		}

		// The emplaceResult now points to a valid index in the vertex array
		indices.emplace_back(static_cast<unsigned int>(emplaceResult.first->second));
	}
};

}
