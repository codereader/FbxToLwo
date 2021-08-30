#pragma once

#include "ModelExporterBase.h"

namespace model
{

class Lwo2Exporter :
	public ModelExporterBase
{
public:
	const std::string& getDisplayName() const;

	// Returns the uppercase file extension this exporter is suitable for
	const std::string& getExtension() const;

    void exportToPath(const std::string& outputPath, const std::string& filename);

private:
	// Export the model file to the given stream
	void exportToStream(std::ostream& stream);
};

}

