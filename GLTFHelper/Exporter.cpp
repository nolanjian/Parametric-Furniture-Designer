#include <GLTFHelper/ExporterImp.h>

namespace PFD
{
    namespace GLTF
    {
        std::shared_ptr<Exporter> Exporter::Create()
        {
            return std::make_shared<ExporterImp>();
        }
    }
}