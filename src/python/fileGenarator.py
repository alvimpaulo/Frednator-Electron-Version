from os import listdir
import cppFileReader as cppfr
import re

templatesLocation = "./src/templates/"
classHeaderFile = "./includes-competition-code/perception/include/fieldDetector2.hpp"

def generateAttrObjSetsString(publicVariables):
    sets = [] #array to store each set string

    for variable in publicVariables:
        sets.append("attrObj.Set(\"{0}\", detector->{0});".format(variable["name"])) #create and append set string
    
    setsString = "\n\t\t".join(sets) #array to string conversion
    return setsString


def generateSetPropertyNamesString(publicVariables):
    properties = []
    for variable in publicVariables:
        #check varaible type and choose expression suffix accordingly
        if(variable["type"] == "int"):
            typeSuffix = "ToNumber().Int32Value()"
        if(variable["type"] == "double"):
            typeSuffix = "ToNumber().DoubleValue()"
        if(variable["type"] == "float"):
            typeSuffix = "ToNumber().FloatValue()"
        if(variable["type"] == "bool"):
            typeSuffix = "ToBoolean()"
            
        #create and append set string    
        properties.append('''
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("{0}") == 0)
            detector->{0} = attrObj[propertyName].operator Napi::Value().{1};
        '''.format(variable["name"], typeSuffix)) 

    propertiesString = "\n\t\t".join(properties) #array to string conversion
    return propertiesString  


def detectorUtilGenerator():
    #function for detectorUtil generation
    publicVariables = cppfr.getPublicVariables(classHeaderFile)
    
    with open("./src/cpp/{0}Util.cc".format(lowercaseDetector), "w+") as newFile: #create new file for the detectorUtil
        #replace placeholders with appropriate values
        newFileContents = re.sub(r"#lowercaseDetector#", lowercaseDetector, fileContents)
        newFileContents = re.sub(r"#uppercaseDetector#", uppercaseDetector, newFileContents)
        newFileContents = re.sub(r"#spacedDetector#", spacedDetector, newFileContents)
        attrObjSetsString = generateAttrObjSetsString(publicVariables)
        newFileContents = re.sub(r"#attrObjSets#", attrObjSetsString, newFileContents)
        setPropertyNamesString = generateSetPropertyNamesString(publicVariables)
        newFileContents = re.sub(r"#setPropertyNames#", setPropertyNamesString, newFileContents)

        #populate file
        newFile.write(newFileContents)


def appjsGenerator():
    with open("src/js/app.js", "r+") as appjsFile:
        appjsContent = appjsFile.read()
        
        templateSections = re.findall(r"(\/\/ *\$start (.+) *\$([\d\D]+)\/\/ *\$end \2 *\$)", fileContents) #separete sections in template file
        for section in templateSections:
            parentSection = re.search(r"^(.+):.+$", section[1]).group(1) #section without :#lowercaseDetector# ending
            mainFileSection = re.search(r"\/\/ *\$start {0}\$([\d\D]+)\/\/ *\$end {0} *\$".format(parentSection), appjsContent) #search for section in CMakeLists.txt  
            mainFileSectionOldContent = mainFileSection.group(1) #whatever was already in the section
            
            #replacing things
            newContent = section[0].replace("#lowercaseDetector#", lowercaseDetector)
            newContent = newContent.replace("#spacedDetector#", spacedDetector)
            

            mainFileSectionNewContent = mainFileSectionOldContent + newContent + "\n\n"
            appjsContent = appjsContent.replace(mainFileSectionOldContent, mainFileSectionNewContent)
        
        #overwrite file
        appjsFile.seek(0)
        appjsFile.write(appjsContent)
        appjsFile.truncate()


def CMakeListsGenerator():
    with open("CMakeLists.txt", "r+") as CMakeListsFile:
        CMakeListsContent = CMakeListsFile.read()

        modulesNumbers = re.findall(r"MODULE_(\d+)_NAME", CMakeListsContent) #determine #NUMBER#
        number = int(max(modulesNumbers)) + 1

        templateSections = re.findall(r"(# *\$start (.+) *\$([\d\D]+)# *\$end \2 *\$)", fileContents) #separete sections in template file    
        for section in templateSections:
            parentSection = re.search(r"^(.+):.+$", section[1]).group(1) #section without :#lowercaseDetector# ending
            
            mainFileSection = re.search(r"# *\$start {0}\$([\d\D]+)# *\$end {0} *\$".format(parentSection), CMakeListsContent) #search for section in CMakeLists.txt  
            mainFileSectionOldContent = mainFileSection.group(1) #whatever was already in the section
            
            #replacing things
            newContent = section[0].replace("#lowercaseDetector#", lowercaseDetector)
            newContent = newContent.replace("#NUMBER#", str(number))

            mainFileSectionNewContent = mainFileSectionOldContent + newContent + "\n\n"
            CMakeListsContent = CMakeListsContent.replace(mainFileSectionOldContent, mainFileSectionNewContent)
        
        #overwrite file
        CMakeListsFile.seek(0)
        CMakeListsFile.write(CMakeListsContent)
        CMakeListsFile.truncate()

def imgCreationjsGenerator():
    with open("src/js/imgCreation.js", "r+") as imgCreationjsFile:
        imgCreationjsContent = imgCreationjsFile.read()
        
        templateSections = re.findall(r"(\/\/ *\$start (.+) *\$([\d\D]+)\/\/ *\$end \2 *\$)", fileContents) #separete sections in template file
        for section in templateSections:
            parentSection = re.search(r"^(.+):.+$", section[1]).group(1) #section without :#lowercaseDetector# ending
            mainFileSection = re.search(r"\/\/ *\$start {0}\$([\d\D]+)\/\/ *\$end {0} *\$".format(parentSection), imgCreationjsContent) #search for section in CMakeLists.txt  
            mainFileSectionOldContent = mainFileSection.group(1) #whatever was already in the section
            
            #replacing things
            newContent = section[0].replace("#lowercaseDetector#", lowercaseDetector)
            newContent = newContent.replace("#spacedDetector#", spacedDetector)
            

            mainFileSectionNewContent = mainFileSectionOldContent + newContent + "\n\n"
            imgCreationjsContent = imgCreationjsContent.replace(mainFileSectionOldContent, mainFileSectionNewContent)
        
        #overwrite file
        imgCreationjsFile.seek(0)
        imgCreationjsFile.write(imgCreationjsContent)
        imgCreationjsFile.truncate()

def imgCreationWorkerjsGenerator():
    with open("src/js/imgCreationWorker.js", "r+") as imgCreationWorkerjsFile:
        imgCreationWorkerjsContent = imgCreationWorkerjsFile.read()
        
        templateSections = re.findall(r"(\/\/ *\$start (.+) *\$([\d\D]+)\/\/ *\$end \2 *\$)", fileContents) #separete sections in template file
        for section in templateSections:
            parentSection = re.search(r"^(.+):.+$", section[1]).group(1) #section without :#lowercaseDetector# ending
            mainFileSection = re.search(r"\/\/ *\$start {0}\$([\d\D]+)\/\/ *\$end {0} *\$".format(parentSection), imgCreationWorkerjsContent) #search for section in CMakeLists.txt  
            mainFileSectionOldContent = mainFileSection.group(1) #whatever was already in the section
            
            #replacing things
            newContent = section[0].replace("#lowercaseDetector#", lowercaseDetector)

            mainFileSectionNewContent = mainFileSectionOldContent + newContent + "\n\n"
            imgCreationWorkerjsContent = imgCreationWorkerjsContent.replace(mainFileSectionOldContent, mainFileSectionNewContent)
        
        #overwrite file
        imgCreationWorkerjsFile.seek(0)
        imgCreationWorkerjsFile.write(imgCreationWorkerjsContent)
        imgCreationWorkerjsFile.truncate()

def indexHtmlGenerator():
    with open("src/index.html", "r+") as indexHtmlFile:
        indexHtmlContent = indexHtmlFile.read()
        
        templateSections = re.findall(r"(<!-- *\$start (.+) *\$-->\n([\d\D]+)<!-- *\$end \2 *\$-->)", fileContents) #separete sections in template file
        for section in templateSections:
            parentSection = re.search(r"^(.+):.+$", section[1]).group(1) #section without :#lowercaseDetector# ending
            mainFileSection = re.search(r"<!-- *\$start {0}\$ *-->([\d\D]+)<!-- *\$end {0} *\$ *-->".format(parentSection), indexHtmlContent) #search for section in CMakeLists.txt  
            mainFileSectionOldContent = mainFileSection.group(1) #whatever was already in the section
            
            #replacing things
            newContent = section[0].replace("#lowercaseDetector#", lowercaseDetector)
            newContent = newContent.replace("#spacedDetector#", spacedDetector)

            mainFileSectionNewContent = mainFileSectionOldContent + newContent + "\n\n"
            indexHtmlContent = indexHtmlContent.replace(mainFileSectionOldContent, mainFileSectionNewContent)
        
        #overwrite file
        indexHtmlFile.seek(0)
        indexHtmlFile.write(indexHtmlContent)
        indexHtmlFile.truncate()

#strings to be replaced
uppercaseDetector = cppfr.getUppercaseDetector(classHeaderFile)
lowercaseDetector = uppercaseDetector[0].lower() + uppercaseDetector[1:]
spacedDetector = uppercaseDetector[0] + re.sub(r"([A-Z])", r" \1", uppercaseDetector[1:])

for file in listdir(templatesLocation):
    with open(templatesLocation + file, "r") as currentFile:
        fileContents = currentFile.read()
        if(file == "templates-detectorUtil.cc"):
            detectorUtilGenerator()
            pass
        if(file == "templates-CMakeLists.txt"):
            CMakeListsGenerator()
            pass
        if(file == "templates-app.js"):
            appjsGenerator()
            pass
        if(file == "templates-imgCreation.js"):
            imgCreationjsGenerator()
            pass
        if(file == "templates-imgCreationWorker.js"):
            imgCreationWorkerjsGenerator()
            pass
        if(file == "templates-index.html"):
            indexHtmlGenerator()
            pass
                    
                    


                    
