from os import listdir, makedirs
from os.path import isdir, isfile, dirname, join, splitext
from typing import TextIO


def formatCode(codeStr : str):
    formatted = " ".join(codeStr.split()).replace(" (", "(").replace(" )", ")")
    if "{" in formatted and "= {" not in formatted:
        start = formatted.find("{")
        end   = formatted.find("}", start)+1
        formatted = formatted[:start] + formatted[end:]
    return formatted.strip(";").strip()


def handleFile(inPath : str):
    outPath = inPath.replace("./", "../../Documentation/CodeDoc/")
    outPath = outPath.replace(".h", ".md")
    makedirs(dirname(outPath), exist_ok=True)

    inLines = open(inPath, "r").readlines()
    outFile = open(outPath, "w")

    for idx, line in enumerate(inLines):
        line = line.strip()
        if line.startswith("// -"):
            headerStart = line.find("- ") + 2
            headerEnd   = line.find(" -", headerStart)
            headerLevel = line[0:headerStart].count("-")
            outFile.write("#"*headerLevel + " " + line[headerStart:headerEnd] + "\n\n")

        elif line.startswith("// *"):
            boldStart = line.find("* ") + 2
            boldEnd   = line.find(" *", boldStart)
            outFile.write("**" + line[boldStart:boldEnd] + "**\n\n")

        elif line.startswith("//"):
            if inLines[idx+1].strip() != "" and "//" not in inLines[idx+1]:
                outFile.write("```cpp\n")
                offset = 1
                indentCount = 0
                while True:
                    formattedCode = formatCode(inLines[idx+offset])
                    if offset <= 1:
                        indentCount = formattedCode.find("(")+1
                    else:
                        outFile.write(" " * indentCount)
                    outFile.write(formattedCode)
                    offset += 1
                    if not formattedCode.endswith(","):
                        break
                    else:
                        outFile.write("\n")
                outFile.write("\n```\n\n" + line[3:] + "\n\n<br>\n\n")
            else:
                outFile.write("**" + line[3:].strip(".") + "**\n\n")

        elif "//" in line:
            codeStart = 0
            codeEnd   = line.find("//")
            textStart = codeEnd + 3
            textEnd   = len(line)
            if (line[textStart:textEnd] == ""):
                outFile.write("```cpp\n" + formatCode(line[codeStart:codeEnd]) + "\n```\n\n")
            else:
                outFile.write("```cpp\n" + formatCode(line[codeStart:codeEnd]) + "\n```\n\n" + line[textStart:textEnd] + "\n\n<br>\n\n")



def handleFilesInDir(directory : str):
    dirContents = listdir(directory)
    for element in dirContents:
        elementPath = join(directory, element)
        if isdir(elementPath):
            handleFilesInDir(elementPath)
        elif isfile(elementPath) and splitext(elementPath)[1] == ".h":
            handleFile(elementPath)


if (__name__ == "__main__"):
    handleFilesInDir("./")
