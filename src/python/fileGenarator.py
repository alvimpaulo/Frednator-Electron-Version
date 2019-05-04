from os import listdir
import cppFileReader as cppfr
import re

templatesLocation = "./src/templates/"
classHeaderFile = "./includes-competition-code/perception/include/lineDetector.hpp"

def generateAttrObjSetsString(publicVariables):
    sets = []
    for variable in publicVariables:
        sets.append("attrObj.Set(\"{0}\", detector->{0});".format(variable["name"]))
    setsString = "\n\t\t".join(sets)
    return setsString

def generateSetPropertyNamesString(publicVariables):
    sets = []
    for variable in publicVariables:
        if(variable["type"] == "int"):
            typeSuffix = "ToNumber().Int32Value()"
        if(variable["type"] == "double"):
            typeSuffix = "ToNumber().DoubleValue()"
        if(variable["type"] == "float"):
            typeSuffix = "ToNumber().FloatValue()"
        if(variable["type"] == "bool"):
            typeSuffix = "ToBoolean()"
        sets.append('''
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("{0}") == 0)
            detector->{0} = attrObj[propertyName].operator Napi::Value().{1};
        '''.format(variable["name"], typeSuffix))
    setsString = "\n\t\t".join(sets)
    return setsString    

for file in listdir(templatesLocation):
    with open(templatesLocation + file, "r") as currentFile:
        fileContents = currentFile.read()
        if(file == "templates-detectorUtil.cc"):
            uppercaseDetector = cppfr.getUppercaseDetector(classHeaderFile)
            lowercaseDetector = uppercaseDetector[0].lower() + uppercaseDetector[1:]
            spacedDetector = uppercaseDetector[0] + re.sub(r"([A-Z])", r" \1", uppercaseDetector[1:])
            publicVariables = cppfr.getPublicVariables(classHeaderFile)
            
            with open("./src/cpp/{0}Util.cc".format(lowercaseDetector), "w+") as newFile:
                newFileContents = re.sub(r"#lowercaseDetector#", lowercaseDetector, fileContents)
                newFileContents = re.sub(r"#uppercaseDetector#", uppercaseDetector, newFileContents)
                newFileContents = re.sub(r"#spacedDetector#", spacedDetector, newFileContents)
                attrObjSetsString = generateAttrObjSetsString(publicVariables)
                newFileContents = re.sub(r"#attrObjSets#", attrObjSetsString, newFileContents)
                setPropertyNamesString = generateSetPropertyNamesString(publicVariables)
                newFileContents = re.sub(r"#setPropertyNames#", setPropertyNamesString, newFileContents)
                newFile.write(newFileContents)
            


