#pragma once

#include <fstream>
#include <map>
#include <vector>
#include "../math/Matrix4.h"

#include "ArbitraryMeshVertex.h"
#include "../FbxSurface.h"

namespace model
{

typedef std::vector<unsigned int> IndexBuffer;

// A Polygon (Triangle) which is part of a model surface
struct ModelPolygon
{
	ArbitraryMeshVertex a;
	ArbitraryMeshVertex b;
	ArbitraryMeshVertex c;
};

class ModelExporterBase
{
protected:
	struct Surface
	{
		std::string materialName;

		// The vertices of this surface
		std::vector<ArbitraryMeshVertex> vertices;

		// The indices connecting the vertices to triangles
		IndexBuffer indices;
	};

	typedef std::map<std::string, Surface> Surfaces;
	Surfaces _surfaces;

public:
	void addSurface(const FbxSurface& incoming, const Matrix4& localToWorld)
	{
		Surface& surface = ensureSurface(incoming.getActiveMaterial());

		Matrix4 invTranspTransform = localToWorld.getFullInverse().getTransposed();

		// Cast succeeded, load the vertices and indices directly into here
		unsigned int indexStart = static_cast<unsigned int>(surface.vertices.size());

		const auto& vertices = incoming.getVertexArray();
		const auto& indices = incoming.getIndexArray();

		if (indices.size() < 3)
		{
			// Reject this index buffer
			throw std::runtime_error("Rejecting model surface with less than 3 indices.");
			return;
		}

		// Transform vertices before inserting them
		for (const auto& meshVertex : vertices)
		{
			// Copy-construct based on the incoming meshVertex, transform the vertex.
			// Transform the normal using the inverse transpose
			// We discard the tangent and bitangent vectors here, none of the exporters is using them.
			surface.vertices.emplace_back(
				localToWorld.transformPoint(meshVertex.vertex),
				invTranspTransform.transformPoint(meshVertex.normal).getNormalised(),
				meshVertex.texcoord,
				meshVertex.colour);
		}

		surface.indices.reserve(surface.indices.size() + indices.size());

		// Incoming polygons are defined in clockwise windings, so reverse the indices
		// as the exporter code expects them to be counter-clockwise.
		for (std::size_t i = 0; i < indices.size() - 2; i += 3)
		{
			surface.indices.push_back(indices[i + 2] + indexStart);
			surface.indices.push_back(indices[i + 1] + indexStart);
			surface.indices.push_back(indices[i + 0] + indexStart);
		}
	}

	void addPolygons(const std::string& materialName, const std::vector<ModelPolygon>& polys)
	{
		Surface& surface = ensureSurface(materialName);

		for (const ModelPolygon& poly : polys)
		{
			unsigned int indexStart = static_cast<unsigned int>(surface.vertices.size());

			ModelPolygon transformed(poly); // copy to transform

#if 0
			transformed.a.vertex = localToWorld.transformPoint(poly.a.vertex);
			transformed.b.vertex = localToWorld.transformPoint(poly.b.vertex);
			transformed.c.vertex = localToWorld.transformPoint(poly.c.vertex);
#endif

			surface.vertices.push_back(transformed.a);
			surface.vertices.push_back(transformed.b);
			surface.vertices.push_back(transformed.c);

			surface.indices.push_back(indexStart);
			surface.indices.push_back(indexStart + 1);
			surface.indices.push_back(indexStart + 2);
		}
	}

private:
	Surface& ensureSurface(const std::string& materialName)
	{
		Surfaces::iterator surf = _surfaces.find(materialName);

		if (surf == _surfaces.end())
		{
			surf = _surfaces.insert(std::make_pair(materialName, Surface())).first;
			surf->second.materialName = materialName;
		}

		return surf->second;
	}
};

}