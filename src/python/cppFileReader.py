import re
    

def getUppercaseDetector(filename):
    with open(filename, "r") as classFile:
        classFileText = classFile.read()
        uppercaseDetector = re.search(r"^class (\S+) {0,}:", classFileText, flags=re.MULTILINE).groups()[0]
    return uppercaseDetector

def getPublicVariables(filename):

    uppercaseDetector = getUppercaseDetector(filename)
    publicVariables = []

    with open(filename, "r") as classFile:
        classFileText = classFile.read()
        publicSections = re.findall(r"(public:[\d\D]+?)((private)|};|$)", classFileText)
        for publicSection in publicSections:
            variablesMatches = re.findall(r"(int|float|double|bool) +(.+);", publicSection[0]);
            for variablesMatch in variablesMatches:
                variablesType = variablesMatch[0]
                variablesNameList = re.split(",", variablesMatch[1])
                for variableName in variablesNameList:
                    variableName = variableName.replace(" ", "")
                    publicVariables.append({"name": variableName, "type":variablesType})
    return publicVariables
    
    """ for member in detectorVariables:
        if(member.access_type == "public"):
            print("Found public variable: {}".format(member.name))
            publicVariables.append(member)
    return publicVariables """


#getPublicVariables("includes-competition-code/perception/include/fieldDetector2.hpp")
