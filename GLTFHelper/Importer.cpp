#include <GLTFHelper/ImporterImp.h>

namespace PFD
{
    namespace GLTF
    {
        std::shared_ptr<Importer> Importer::Create()
        {
            return std::make_shared<ImporterImp>();
        }
    }
}
