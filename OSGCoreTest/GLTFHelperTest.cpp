
#include <gtest\gtest.h>
#include <GLTFHelper\Importer.h>
#include <Commom\STLIncluding.h>

using namespace PFD::GLTF;

TEST(GLTFHelper, Import1) {
	std::string strRoot = "D:\\glTF-Sample-Models\\2.0";
	for (auto itr : std::filesystem::recursive_directory_iterator(strRoot))
	{
		if (itr.is_regular_file())
		{
			try
			{
				std::filesystem::path p = itr.path();
				std::wstring ext = p.extension().wstring();
				if (ext == L".gltf" || ext == L".glb")
				{
					if (p.string().find("Draco") != std::string::npos)
					{
						continue;
					}

					std::cout << p.string() << std::endl;

					std::shared_ptr<Importer> pImporter = Importer::Create();
					auto scene = pImporter->Load(p.wstring());
					bool bCheck = scene.get() != nullptr;
					EXPECT_EQ(bCheck, true);
					bCheck ? GTEST_SUCCESS_(p.string().c_str()) : GTEST_NONFATAL_FAILURE_(p.string().c_str());


					std::cout << (bCheck ? "Success" : "fail") << std::endl;

					if (!bCheck)
					{
						FAIL() << p.string();
					}
					else
					{
						SUCCEED() << p.string();
					}
				}

			}
			catch (const std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
			}
		}
	}


}