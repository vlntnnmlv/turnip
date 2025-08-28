import os
import re

PROJECT_ROOT_PATH = "/Users/valentinnamleev/Code/turnip/"
COMPONENT_HEADERS_PATH = os.path.join(PROJECT_ROOT_PATH, "include/ecs/components/")
CONSTRUCTOR_PATTERN = r"(\()((.|\s)*?)(\{)(\})"
CONSTRUCTOR_DECLARATION_PATTERN = r"(\()((.|\s)*?)(\))"

def getComponentConstructor(componentHeaderText):

    componentName = componentHeaderText.split("struct")[1].split(":")[0].strip()
    constructorMatch = re.search(componentName + CONSTRUCTOR_PATTERN, componentHeaderText)

    if constructorMatch:
        return componentName, constructorMatch.group(0)
    else:
        return componentName, None

projectRootPath = os.getcwd()
componentHeaders = os.listdir(COMPONENT_HEADERS_PATH)

for componentHeader in componentHeaders:
    with open(os.path.join(COMPONENT_HEADERS_PATH, componentHeader), "r+") as f:
        allText = f.read()

        componentName, constructor = getComponentConstructor(allText)

        if constructor is None:
            print(componentName, "has no constructor!")
            continue

        pattern = componentName + CONSTRUCTOR_DECLARATION_PATTERN
        constructorDeclarationMatch = re.search(pattern, constructor)

        if constructorDeclarationMatch:
            # print(componentName, ":", constructorDeclarationMatch.group(0))
            # constructorDeclaration = constructorDeclarationMatch.group(0)
            pass
        else:
            print(componentName, ": ERROR!")
            continue



