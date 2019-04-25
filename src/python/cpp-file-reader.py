from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser

uppercaseDetector = "YellowDetector"

# Find out the c++ parser
generator_path, generator_name = utils.find_xml_generator()

# Configure the xml generator
xml_generator_config = parser.xml_generator_configuration_t(
    xml_generator_path=generator_path,
    xml_generator=generator_name,
    include_paths=[
        "/home/paulo/.cmake-js/node-x64/v10.15.2/include/node",
        "/usr/local/include",
        "/usr/local/include/opencv",
        "/home/paulo/frednator-2/node_modules/node-addon-api",
        "/home/paulo/nao/devtools/sdk/naoqi-sdk-2.1.4.13-linux64/include",
        "/usr/include",
        "/home/paulo/frednator-2/includes-competition-code/motion/include",
        "/home/paulo/frednator-2/includes-competition-code/motion/runswift",
        "/home/paulo/frednator-2/includes-competition-code/unboard",
        "/home/paulo/frednator-2/includes-competition-code/common",
        "/home/paulo/frednator-2/includes-competition-code/module",
    ])

# The c++ file we want to parse
filename = "./includes-competition-code/perception/include/yellowDetector.hpp"

decls = parser.parse([filename], xml_generator_config)
global_namespace = declarations.get_global_namespace(decls)
detectorClass = global_namespace.classes(name=uppercaseDetector)
detectorVariables = detectorClass[0].variables()
for member in detectorVariables:
    if(member.access_type == "public"):
        print(member.name)
#ns = global_namespace.namespace("ns")