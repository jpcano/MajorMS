from conan import ConanFile


class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("cxxopts/3.2.0")
        self.requires("utfcpp/4.0.5")
        self.requires("nlohmann_json/3.11.3")
