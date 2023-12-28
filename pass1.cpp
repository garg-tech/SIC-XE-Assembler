/*Code for pass1*/
#include "utility.cpp" /*conatins all important headers*/
#include "tables.cpp"

using namespace std;

/*Variable to keep persisted*/
string fileName;
bool error_flag = false;
int program_length;
string *BLocksNumToName;

// Pass 1 implementation
void pass1()
{
    ifstream sourceFile; // begin
    ofstream intermediateFile, errorFile;

    sourceFile.open(fileName);
    if (!sourceFile)
    {
        cout << "Unable to open file: " << fileName << endl;
        exit(1);
    }

    intermediateFile.open("intermediate_" + fileName);
    if (!intermediateFile)
    {
        cout << "Unable to open file: intermediate_" << fileName << endl;
        exit(1);
    }
    writeToFile(intermediateFile, "Line\tAddress\tLabel\tOPCODE\tOPERAND");
    errorFile.open("error_" + fileName);
    if (!errorFile)
    {
        cout << "Unable to open file: error_" << fileName << endl;
        exit(1);
    }
    writeToFile(errorFile, "************PASS1************");

    string fileLine;
    string writeData, writeDataSuffix = "", writeDataPrefix = "";
    int index = 0;

    string currentBlockName = "DEFAULT";
    int currentBlockNumber = 0;
    int totalBlocks = 1;

    bool statusCode;
    string label, opcode, operand, comment;
    string tempOperand;

    int startAddress, LOCCTR, saveLOCCTR, lineNumber, lastDeltaLOCCTR, lineNumberDelta = 0;
    lineNumber = 0;
    lastDeltaLOCCTR = 0;

    getline(sourceFile, fileLine);
    lineNumber += 5;
    while (checkCommentLine(fileLine))
    {
        writeData = to_string(lineNumber) + "\t" + fileLine;
        writeToFile(intermediateFile, writeData);
        getline(sourceFile, fileLine); // read first input line
        lineNumber += 5;
        index = 0;
    }

    readFirstNonWhiteSpace(fileLine, index, statusCode, label);
    readFirstNonWhiteSpace(fileLine, index, statusCode, opcode);

    if (opcode == "START")
    {
        readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
        readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
        startAddress = stringHexToInt(operand);
        LOCCTR = startAddress;
        writeData = to_string(lineNumber) + "\t" + intToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment;
        writeToFile(intermediateFile, writeData); // Write file to intermediate file

        getline(sourceFile, fileLine); // Read next line
        lineNumber += 5;
        index = 0;
        readFirstNonWhiteSpace(fileLine, index, statusCode, label);  // Parse label
        readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); // Parse OPCODE
    }
    else
    {
        startAddress = 0;
        LOCCTR = 0;
    }

    while (opcode != "END")
    {
        if (!checkCommentLine(fileLine))
        {
            if (label != "")
            { // Label found
                if (SYMTAB[label].exists == 'n')
                {
                    SYMTAB[label].name = label;
                    SYMTAB[label].address = intToStringHex(LOCCTR);
                    SYMTAB[label].relative = 1;
                    SYMTAB[label].exists = 'y';
                    SYMTAB[label].blockNumber = currentBlockNumber;
                }
                else
                {
                    writeData = "Line: " + to_string(lineNumber) + " : Duplicate symbol for '" + label + "'. Previously defined at " + SYMTAB[label].address;
                    writeToFile(errorFile, writeData);
                    error_flag = true;
                }
            }
            if (OPTAB[getRealOpcode(opcode)].exists == 'y')
            { // Search for opcode in OPTAB
                if (OPTAB[getRealOpcode(opcode)].format == 3)
                {
                    LOCCTR += 3;
                    lastDeltaLOCCTR += 3;
                    if (getFlagFormat(opcode) == '+')
                    {
                        LOCCTR += 1;
                        lastDeltaLOCCTR += 1;
                    }
                    if (getRealOpcode(opcode) == "RSUB")
                    {
                        operand = " ";
                    }
                    else
                    {
                        readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                        if (operand[operand.length() - 1] == ',')
                        {
                            readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                            operand += tempOperand;
                        }
                    }
                }
                else if (OPTAB[getRealOpcode(opcode)].format == 1)
                {
                    operand = " ";
                    LOCCTR += OPTAB[getRealOpcode(opcode)].format;
                    lastDeltaLOCCTR += OPTAB[getRealOpcode(opcode)].format;
                }
                else
                {
                    LOCCTR += OPTAB[getRealOpcode(opcode)].format;
                    lastDeltaLOCCTR += OPTAB[getRealOpcode(opcode)].format;
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    if (operand[operand.length() - 1] == ',')
                    {
                        readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                        operand += tempOperand;
                    }
                }
            }
            else if (opcode == "WORD")
            {
                readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                LOCCTR += 3;
                lastDeltaLOCCTR += 3;
            }
            else if (opcode == "RESW")
            {
                readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                LOCCTR += 3 * stoi(operand);
                lastDeltaLOCCTR += 3 * stoi(operand);
            }
            else if (opcode == "RESB")
            {
                readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                LOCCTR += stoi(operand);
                lastDeltaLOCCTR += stoi(operand);
            }
            else if (opcode == "BYTE")
            {
                readByteOperand(fileLine, index, statusCode, operand);
                if (operand[0] == 'X')
                {
                    LOCCTR += (operand.length() - 3) / 2;
                    lastDeltaLOCCTR += (operand.length() - 3) / 2;
                }
                else if (operand[0] == 'C')
                {
                    LOCCTR += operand.length() - 3;
                    lastDeltaLOCCTR += operand.length() - 3;
                }
            }
            else if (opcode == "BASE")
            {
                readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
            }
            else if (opcode == "USE")
            {
                readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                BLOCKS[currentBlockName].LOCCTR = intToStringHex(LOCCTR);

                if (BLOCKS[operand].exists == 'n')
                {
                    BLOCKS[operand].exists = 'y';
                    BLOCKS[operand].name = operand;
                    BLOCKS[operand].number = totalBlocks++;
                    BLOCKS[operand].LOCCTR = "0";
                }
                currentBlockNumber = BLOCKS[operand].number;
                currentBlockName = BLOCKS[operand].name;
                LOCCTR = stringHexToInt(BLOCKS[operand].LOCCTR);
            }
            else
            {
                readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                writeData = "Line: " + to_string(lineNumber) + " : Invalid OPCODE. Found " + opcode;
                writeToFile(errorFile, writeData);
                error_flag = true;
            }
            readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
            writeData = writeDataPrefix + to_string(lineNumber) + "\t" + intToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
            writeDataPrefix = "";
            writeDataSuffix = "";
        }
        else
        {
            writeData = to_string(lineNumber) + "\t" + fileLine;
        }
        writeToFile(intermediateFile, writeData);

        BLOCKS[currentBlockName].LOCCTR = intToStringHex(LOCCTR); // Update LOCCTR of block after every instruction
        getline(sourceFile, fileLine);                            // Read next line
        lineNumber += 5 + lineNumberDelta;
        lineNumberDelta = 0;
        index = 0;
        lastDeltaLOCCTR = 0;
        readFirstNonWhiteSpace(fileLine, index, statusCode, label);  // Parse label
        readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); // Parse OPCODE
    }

    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
    readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);

    currentBlockName = "DEFAULT";
    currentBlockNumber = 0;
    LOCCTR = stringHexToInt(BLOCKS[currentBlockName].LOCCTR);

    writeData = to_string(lineNumber) + "\t" + intToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
    writeToFile(intermediateFile, writeData);

    int LocctrArr[totalBlocks];
    BLocksNumToName = new string[totalBlocks];
    for (auto const &it : BLOCKS)
    {
        LocctrArr[it.second.number] = stringHexToInt(it.second.LOCCTR);
        BLocksNumToName[it.second.number] = it.first;
    }

    for (int i = 1; i < totalBlocks; i++)
    {
        LocctrArr[i] += LocctrArr[i - 1];
    }

    for (auto const &it : BLOCKS)
    {
        if (it.second.startAddress == "?")
        {
            BLOCKS[it.first].startAddress = intToStringHex(LocctrArr[it.second.number - 1]);
        }
    }

    program_length = LocctrArr[totalBlocks - 1] - startAddress;

    sourceFile.close();
    intermediateFile.close();
    errorFile.close();
}