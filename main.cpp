#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Definirea structurii WorkflowInfo în scop global
struct WorkflowInfo {
    string name = "";
    string title_step = "";
    string text_step = "";
    string text_input_step = "";
    float number_input_step = 0;
    float calculus_step = 0;
    string display_step = "";
    string text_file_input_step = "";
    string csv_file_input_step = "";
    string output_step = "";
    string end_step = "";
};

// Vector global de structuri WorkflowInfo
vector<WorkflowInfo> workflowInfos;

int workflowCount = 0; // Contor global pentru numărul de workflow-uri

int totalStepExecutions = 0; // Contor global pentru numărul total de execuții ale pașilor
map<int, string> fileNamesMap; // Mapă pentru a stoca numele fișierelor create la fiecare pas

class Step {
protected:
    string description; // Descrierea pentru fiecare pas

public:
    Step(const string& desc) : description(desc) {}
    virtual void execute() = 0;
    virtual ~Step() {}

    string getDescription() const {
        return description;
    }

    void displayStepNumber() const {
        cout << "\nStep " << totalStepExecutions << ": " << getDescription() << endl;
    }
};

// Clasele TitleStep, TextStep, TextInputStep, NumberInputStep, CalculusStep rămân neschimbate
class TitleStep : public Step {
private:
    string title;
    string subtitle;

public:
    WorkflowInfo currentFlow;  // Membra pentru a stoca informațiile

    TitleStep() : Step("Title Step - Enter a title and a subtitle.") {}

    void setTitle() {
        cout << "Enter title: ";
        getline(cin, title);
    }

    void setSubtitle() {
        cout << "Enter subtitle: ";
        getline(cin, subtitle);
    }

    void execute() override {
    displayStepNumber();
    setTitle();
    setSubtitle();

    currentFlow.title_step = title + " " + subtitle;  // Actualizează informațiile în structura locală
    // Adaugă informațiile la vectorul global (workflowInfos)
    workflowInfos.push_back(currentFlow);
    
    cout << "\nTitle: " << title << "\nSubtitle: " << subtitle << endl << endl;
}

};


class TextStep : public Step {
private:
    string title;
    string copy;

public:

    WorkflowInfo currentFlow;  // Membra pentru a stoca informațiile

    TextStep() : Step("Text Step - Display and input text.") {}

    void setTitle() {
        cout << "Enter title: ";
        getline(cin, title);
    }

    void setText() {
        cout << "Enter text: ";
        getline(cin, copy);
    }

    void execute() override {
        displayStepNumber();
        setTitle();       // Setează titlul de la tastatură
        setText();        // Setează textul de la tastatură
        currentFlow.text_step = title + " " + copy;  // Actualizează informațiile în structura locală
        // Adaugă informațiile la vectorul global (workflowInfos)
        workflowInfos.push_back(currentFlow);
        cout << "\nTitle: " << title << "\nText: " << copy << endl << endl;
    }
};

class TextInputStep : public Step {
private:
    string description;
    string textInput;

public:

    WorkflowInfo currentFlow;  // Membra pentru a stoca informațiile

    TextInputStep() : Step("Text Input Step - Enter your text.") {}
    
    void setDescription() {
        cout << "Enter description: ";
        getline(cin, description);
    }

    void setInputPrompt() {
        cout << "Enter input prompt: ";
        getline(cin, textInput);
    }

    void execute() override {
        displayStepNumber();
        setDescription(); // Setează descrierea de la tastatură
        setInputPrompt(); // Setează promptul de la tastatură
        currentFlow.text_input_step = description + " " + textInput;  // Actualizează informațiile în structura locală
        // Adaugă informațiile la vectorul global (workflowInfos)
        workflowInfos.push_back(currentFlow);

        cout << "\nDescription: " << description << "\nInput prompt: " << textInput << endl <<endl;
    }
};

class NumberInputStep : public Step {
private:
    float numberInput;
    static map<int, float> results;

public:

    WorkflowInfo currentFlow ;  // Membra pentru a stoca informațiile

    NumberInputStep() : Step("Number Input Step - Enter a number.") {}

    void setNumber() {
        cout << "Enter a number: ";
        cin >> numberInput;
        cin.ignore();
        results[totalStepExecutions] = numberInput;
    }

    void execute() override {
        displayStepNumber();
        setNumber();
        currentFlow.number_input_step = numberInput;  // Actualizează informațiile în structura locală
        // Adaugă informațiile la vectorul global (workflowInfos)
        workflowInfos.push_back(currentFlow);
        cout << "Number entered: " << numberInput << endl;
    }

    static float getNumberResult(int executionNumber) {
        if (results.find(executionNumber) != results.end()) {
            return results[executionNumber];
        }
        cout << "No number input found for execution number. " << executionNumber << endl;
        return 0;
    }
};

map<int, float> NumberInputStep::results = {};

class CalculusStep : public Step {
public:

     WorkflowInfo currentFlow;  // Membra pentru a stoca informațiile

    CalculusStep() : Step("Calculus Step - Perform a calculation.") {}

    void execute() override {
        displayStepNumber();

        int stepIndex1, stepIndex2;
        cout << "Enter the number of the first step for calculation: ";
        cin >> stepIndex1;
        cin.ignore();
        cout << "Enter the number of the second step for calculation: ";
        cin >> stepIndex2;
        cin.ignore();

        char operation;
        cout << "Enter the operation ('+', '-', '*', '/'): ";
        cin >> operation;
        cin.ignore();

        float result1 = NumberInputStep::getNumberResult(stepIndex1);
        float result2 = NumberInputStep::getNumberResult(stepIndex2);
        float calculationResult = performCalculation(result1, result2, operation);

        currentFlow.calculus_step = calculationResult;  // Actualizează informațiile în structura locală
        // Adaugă informațiile la vectorul global (workflowInfos)
        workflowInfos.push_back(currentFlow);
        cout << "Result: " << calculationResult << endl;
    }

private:
    float performCalculation(float num1, float num2, char op) {
        switch(op) {
            case '+': return num1 + num2;
            case '-': return num1 - num2;
            case '*': return num1 * num2;
            case '/': return num2 != 0 ? num1 / num2 : 0;
            default: cout << "Unknown operation!" << endl; return 0;
        }
    }
};

class TextFileStep : public Step {
private:
    string fileName;
    string content;

public:

    WorkflowInfo currentFlow;  // Membra pentru a stoca informațiile

    TextFileStep() : Step("Text File Step - Create or modify a text file.") {}

    void setFileName() {
        cout << "Enter text file name: ";
        getline(cin, fileName);
    }

    void setContent() {
        cout << "Enter content to add to the text file: ";
        getline(cin, content);
    }

    void execute() override {
        displayStepNumber();
        setFileName();
        setContent();

        ofstream file(fileName, ios::app);
        if (file) {
            currentFlow.text_file_input_step = content;  // Actualizează informațiile în structura locală
            // Adaugă informațiile la vectorul global (workflowInfos)
            workflowInfos.push_back(currentFlow);
            file << content << endl;
            cout << "Content added to " << fileName << endl;
            fileNamesMap[totalStepExecutions + 1] = fileName;
        } else {
            cout << "Unable to open file " << fileName << endl;
        }
        file.close();
    }
};

class CsvFileStep : public Step {
private:
    string fileName;
    string sentence;

public:

    WorkflowInfo currentFlow;  // Membra pentru a stoca informațiile

    CsvFileStep() : Step("CSV File Step - Create or modify a CSV file.") {}

    void setFileName() {
        cout << "Enter CSV file name: ";
        getline(cin, fileName);
    }

    void getSentenceFromUser() {
        cout << "Enter a sentence (words separated by spaces): ";
        getline(cin, sentence);
    }

    void writeSentenceToCsv(ofstream& file) {
        istringstream iss(sentence);
        string word;
        string lineContent;

        while (iss >> word) {
            if (!lineContent.empty()) {
                lineContent += ",";
            }
            lineContent += word;
        }

        if (!lineContent.empty()) {
            file << lineContent << "\n";
        }
    }

    void execute() override {
        displayStepNumber();
        setFileName();
        getSentenceFromUser();

        ofstream file(fileName, ios::app);
        if (file) {
            writeSentenceToCsv(file);
            currentFlow.csv_file_input_step = sentence;  // Actualizează informațiile în structura locală
            // Adaugă informațiile la vectorul global (workflowInfos)
            workflowInfos.push_back(currentFlow);
            cout << "Sentence added to " << fileName << endl;
            fileNamesMap[totalStepExecutions + 1] = fileName;
        } else {
            cout << "Unable to open file " << fileName << endl;
        }
        file.close();
    }
};

class DisplayStep : public Step {
public:
    DisplayStep() : Step("Display Step - Display the content of a specific file.") {}

    void execute() override {
        displayStepNumber();

        int stepIndex;
        cout << "Enter the step number to display its associated file: ";
        cin >> stepIndex;
        cin.ignore();

        auto it = fileNamesMap.find(stepIndex + 1);
        if (it != fileNamesMap.end()) {
            string fileName = it->second;
            ifstream file(fileName);
            if (file) {
                cout << "Displaying contents of " << fileName << ":" << endl;
                string line;
                while (getline(file, line)) {
                    for (char& ch : line) {
                        if (ch == ',') ch = ' '; // Înlocuirea fiecărei virgule cu un spațiu
                    }
                    cout << line << endl;
                }
                file.close();
            } else {
                cout << "File " << fileName << " not found or is empty." << endl;
            }
        } else {
            cout << "No file associated with step " << stepIndex << "." << endl;
        }
    }
};

class OutputStep : public Step {
private:
    string outputFileName;
    string title;
    string description;
    vector<int> stepsToInclude;

public:
    OutputStep() : Step("Output Step - Generate a text file with specified content.") {}

    void setOutputFileName() {
        cout << "Enter the name of the output file: ";
        getline(cin, outputFileName);
    }

    void setTitle() {
        cout << "Enter title for the output file: ";
        getline(cin, title);
    }

    void setDescription() {
        cout << "Enter description for the output file: ";
        getline(cin, description);
    }

    void setStepsToInclude() {
        cout << "Enter step numbers to include in the output file (end with -1): ";
        int stepNum;
        while (true) {
            cin >> stepNum;
            if (stepNum == -1) break;
            stepsToInclude.push_back(stepNum);
        }
        cin.ignore(); // Pentru a curăța buffer-ul
    }

    void execute() override {
        displayStepNumber();
        setOutputFileName();
        setTitle();
        setDescription();
        setStepsToInclude();

        ofstream outputFile(outputFileName);
        if (outputFile) {
            outputFile << "Title: " << title << "\n";
            outputFile << "Description: " << description << "\n\n";

            for (int stepNum : stepsToInclude) {
                auto it = fileNamesMap.find(stepNum);
                if (it != fileNamesMap.end()) {
                    string fileName = it->second;
                    outputFile << "Content from step " << stepNum << " (" << fileName << "):" << endl;
                    ifstream inputFile(fileName);
                    if (inputFile) {
                        string line;
                        while (getline(inputFile, line)) {
                            outputFile << line << endl;
                        }
                        inputFile.close();
                    }
                    outputFile << "\n";
                }
            }

            cout << "Output file generated: " << outputFileName << endl;
        } else {
            cout << "Unable to create output file " << outputFileName << endl;
        }
        outputFile.close();
    }
};

class EndStep : public Step {
public:
    EndStep() : Step("End Step - Marks the end of the workflow.") {}

    void execute() override {
        displayStepNumber();
        cout << "This is the end of the workflow. No further actions required." << endl;
    }
};

class Workflow {
private:
    string name;
    vector<Step*> steps;

public:
    Workflow() {}

    void setWorkflowName() {
        cout << "Enter the name of the workflow: ";
        getline(cin, name);
    }

    string getWorkflowName() const {
        return name;
    }

    void addStep(Step* step) {
        steps.push_back(step);
    }

    void run() {
        for (auto& step : steps) {
            char userChoice;
            bool firstExecution = true;

            do {
                if (firstExecution) {
                    cout << step->getDescription() << " Would you like to execute this step? (Y/N): ";
                    firstExecution = false;
                } else {
                    cout << "Would you like to execute this step again? (Y/N): ";
                }

                cin >> userChoice;
                cin.ignore();

                if (userChoice == 'Y' || userChoice == 'y') {
                    totalStepExecutions++;
                    step->execute();
                } else if (userChoice != 'N' && userChoice != 'n') {
                    cout << "Invalid input. Please enter Y or N." << endl;
                }
            } while (userChoice != 'N' && userChoice != 'n');
        }
        cout << "Workflow completed." << endl;
    }

    ~Workflow() {
        for (auto& step : steps) {
            delete step;
        }
    }
};

void addWorkflowSteps(Workflow& workflow) {
    workflow.addStep(new TitleStep());
    workflow.addStep(new TextStep());
    workflow.addStep(new TextInputStep());
    workflow.addStep(new NumberInputStep());
    workflow.addStep(new CalculusStep());
    workflow.addStep(new TextFileStep());
    workflow.addStep(new CsvFileStep());
    workflow.addStep(new DisplayStep());
    workflow.addStep(new OutputStep());
    workflow.addStep(new EndStep());
}

void startNewWorkflow() {
    cout << "Starting a new workflow..." << endl;
    Workflow myWorkflow;
    myWorkflow.setWorkflowName();
    addWorkflowSteps(myWorkflow);
    
    WorkflowInfo myWorkflowInfo;  // Creează o structură locală
    myWorkflowInfo.name = myWorkflow.getWorkflowName();
    myWorkflow.run();

    // Adaugă noul workflow în vectorul global
    workflowInfos.push_back(myWorkflowInfo);
    totalStepExecutions++;
}



int main() {

    cout<<"Welcome on the flow application! These are the steps you can choose from: \n\n";
    cout << "1. TITLE step: title (string), subtitle (string) - Adds a title step to the workflow." << endl;
    cout << "2. TEXT step: title (string), copy (string) - Adds a text step to the workflow." << endl;
    cout << "3. TEXT INPUT step: description (string), text input (string) - Adds a text input step to the workflow." << endl;
    cout << "4. NUMBER INPUT step: description (string), number input (float) - Adds a number input step to the workflow." << endl;
    cout << "5. CALCULUS step: steps (integer), operation (string) - Adds a calculus step to the workflow." << endl;
    cout << "6. DISPLAY steps: step (integer) - Adds a display step to the workflow." << endl;
    cout << "7. TEXT FILE input step: description, file_name - Adds a text file input step to the workflow." << endl;
    cout << "8. CSV FILE input step: description, file_name - Adds a CSV file input step to the workflow." << endl;
    cout << "9. OUTPUT step: step (integer), name of file (string), title (text), description (text) - Adds an output step to the workflow." << endl;
    cout << "10. END step - Marks the end of the workflow.\n" << endl;

    int option ;
    cout<<"Press a button for une of the following options: "<<endl;
    cout<<"1. Start a new workflow"<<endl;
    cout<<"2. Run an existing workflow"<<endl;
    cout<<"3. Delete a workflow"<<endl;
    cout<<"4. Close the application"<<endl;
    cin >> option ;
    cin.ignore();
    
    while (option != 4) {
        switch (option) {
            case 1:
                startNewWorkflow();
                break;
            case 2:
                cout << "Run an existing workflow" << endl;
                // Implementează logica pentru rularea unui workflow existent
                break;
            case 3:
                cout << "Delete a workflow" << endl;
                // Implementează logica pentru ștergerea unui workflow
                break;
            case 4:
                cout << "Close the application" << endl;
                break;
            default:
                cout << "Invalid option. Please enter a valid option." << endl;
                break;
        }

        // Ia o altă opțiune de la utilizator pentru a menține meniul activ
        cout << "Press a button for one of the following options: " << endl;
        cout << "1. Start a new workflow" << endl;
        cout << "2. Run an existing workflow" << endl;
        cout << "3. Delete a workflow" << endl;
        cout << "4. Close the application" << endl;
        cin >> option;
        cin.ignore();
    }

    // Workflow myWorkflow("My First Workflow");

    // myWorkflow.addStep(new TitleStep());
    // myWorkflow.addStep(new TextStep());
    // myWorkflow.addStep(new TextInputStep());
    // myWorkflow.addStep(new NumberInputStep());
    // myWorkflow.addStep(new CalculusStep());
    // myWorkflow.addStep(new TextFileStep()); 
    // myWorkflow.addStep(new CsvFileStep());
    // myWorkflow.addStep(new DisplayStep());
    // myWorkflow.addStep(new OutputStep());
    // myWorkflow.addStep(new EndStep());

    // myWorkflow.run();

    // cout << "WorkflowInfo content:" << endl;
    // cout << "Name: " << myWorkflowInfo.name << endl;
    // cout << "Title Step: " << myWorkflowInfo.title_step << endl;
    // cout << "Text Step: " << myWorkflowInfo.text_step << endl;
    // cout << "Text Input Step: " << myWorkflowInfo.text_input_step << endl;
    // cout << "Number Input Step: " << myWorkflowInfo.number_input_step << endl;
    // cout << "Calculus Step: " << myWorkflowInfo.calculus_step << endl;
    // cout << "Text File Input Step: " << myWorkflowInfo.text_file_input_step << endl;
    // cout << "CSV File Input Step: " << myWorkflowInfo.csv_file_input_step << endl;
    // cout << "Output Step: " << myWorkflowInfo.output_step << endl;
    // cout << "End Step: " << myWorkflowInfo.end_step << endl;

    for (const auto& workflowInfo : workflowInfos) {
                        cout << "Workflow: " << workflowInfo.name << endl;
                        cout << "Title Step: " << workflowInfo.title_step << endl;
                        cout << "Text Step: " << workflowInfo.text_step << endl;
                        cout << "Text Input Step: " << workflowInfo.text_input_step << endl;
                        cout << "Number Input Step: " << workflowInfo.number_input_step << endl;
                        cout << "Calculus Step: " << workflowInfo.calculus_step << endl;
                        cout << "Text File Input Step: " << workflowInfo.text_file_input_step << endl;
                        cout << "CSV File Input Step: " << workflowInfo.csv_file_input_step << endl;
                        cout << "Output Step: " << workflowInfo.output_step << endl;
                        cout << "End Step: " << workflowInfo.end_step << endl;
    }

    
    return 0;
}
