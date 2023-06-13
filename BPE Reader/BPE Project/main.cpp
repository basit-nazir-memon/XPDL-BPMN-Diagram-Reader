#include <iostream>
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

class activityElement {
    string type;
    string id;
    float time;
    int outgoingPaths;
public:
    activityElement() {
        type = "";
        id = "";
        time = 0;
        outgoingPaths = 0;
    }
    activityElement(string type, string id, float time) {
        this->type = type;
        this->id = id;
        this->time = time;
        outgoingPaths = 0;
    }
    string getType() const {
        return type;
    }

    string getId() const {
        return id;
    }

    float getTime() const {
        return time;
    }
    void updateOutgoingPath() {
        outgoingPaths += 1;
    }
    int getPath() { return outgoingPaths; }
};

class diagram {
    activityElement** activities;
    int activityCount;
    int activityMax;
    double time;
public:
    diagram() {
        activities = new activityElement * [50];
        activityCount = 0;
        activityMax = 50;
        time = 0;
    }
    diagram(int size) {
        activities = new activityElement * [size];
        activityCount = 0;
        activityMax = size;
        time = 0;
    }
    void addActivity(string type, string id, int time) {
        activities[activityCount++] = new activityElement(type, id, time);
    }
    void addTime(string id) {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i]->getId() == id) {
                time += activities[i]->getTime();
                return;
            }
        }
    }
    void addTime(string id, float partition) {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i]->getId() == id) {
                time += activities[i]->getTime()*partition;
                return;
            }
        }
    }

    int getTime(string id) {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i]->getId() == id) {
                return activities[i]->getTime();
            }
        }
    }

    int getTime() {
        return time;
    }

    void updateOutgoingPath(string id) {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i]->getId() == id) {
                activities[i]->updateOutgoingPath();
                return;
            }
        }
    }

    string getType(string id) {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i]->getId() == id) {
                return activities[i]->getType();
            }
        }
    }

    void addTime(float t) {
        time += t;
    }

    int getPaths(string id) {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i]->getId() == id) {
                return activities[i]->getPath();
            }
        }
        return 0;
    }

    void printAnalytics() {

    }
};




xml_node<>* parallelPath(xml_node<>* e, diagram& d, int& time) {
    cout << "Entered Parallel Path\n";
    string from, to;
    int tmp = 0, temp2 = 0;
    for (xml_node<>* element = e; element; element = element->next_sibling()) {
        from = element->first_attribute("From")->value();
        to = element->first_attribute("To")->value();
        time += d.getTime(from);
        if (d.getPaths(from) > 1) {
            cout << "Num Paths: " << d.getPaths(from) << endl;
            for (int i = d.getPaths(from); i > 0; i--) {
                if (element->first_node("Condition")->first_node("Expression")) {
                    float part = 0;
                    string str = element->first_node("Condition")->first_node("Expression")->value();
                    if (str == "0") { part = 0; }
                    if (str == "0.1") { part = 0.1; }
                    if (str == "0.2") { part = 0.2; }
                    if (str == "0.3") { part = 0.3; }
                    if (str == "0.4") { part = 0.4; }
                    if (str == "0.5") { part = 0.5; }
                    if (str == "0.6") { part = 0.6; }
                    if (str == "0.7") { part = 0.7; }
                    if (str == "0.8") { part = 0.8; }
                    if (str == "0.9") { part = 0.9; }
                    if (str == "1") { part = 1; }
                    //element = multiplePath(element->next_sibling(), part, d);
                }
                else {
                    element = parallelPath(element->next_sibling(), d, tmp);
                    if (tmp > temp2) {
                        temp2 = tmp;
                    }
                    if (i == 1) {
                        d.addTime(temp2);
                    }
                }
            }
        }
        if (d.getType(to) == "Converging") {
            cout << "Left Parallel Path\n";
            return element;
        }
    }
}



xml_node<>* multiplePath(xml_node<>* e, float partition, diagram& d) {
    cout << "Entered Multiple Path\n";
    string from, to;
    int tmp = 0, temp2 = 0;
    for (xml_node<>* element = e; element; element = element->next_sibling()) {
        from = element->first_attribute("From")->value();
        cout << d.getType(from) << endl;
        to = element->first_attribute("To")->value();
        d.addTime(from, partition);
        if (d.getPaths(from) > 1) {
            cout << "Num Paths: " << d.getPaths(from) << endl;
            for (int i = d.getPaths(from); i > 0; i--) {
                if (element->first_node("Condition")->first_node("Expression")) {
                    float part = 0;
                    string str = element->first_node("Condition")->first_node("Expression")->value();
                    if (str == "0") { part = 0; }
                    if (str == "0.1") { part = 0.1; }
                    if (str == "0.2") { part = 0.2; }
                    if (str == "0.3") { part = 0.3; }
                    if (str == "0.4") { part = 0.4; }
                    if (str == "0.5") { part = 0.5; }
                    if (str == "0.6") { part = 0.6; }
                    if (str == "0.7") { part = 0.7; }
                    if (str == "0.8") { part = 0.8; }
                    if (str == "0.9") { part = 0.9; }
                    if (str == "1") { part = 1; }
                    element = multiplePath(element->next_sibling(), part, d);
                }
                else {
                    element = parallelPath(element->next_sibling(), d, tmp);
                    if (tmp > temp2) {
                        temp2 = tmp;
                    }
                    if (i == 1) {
                        d.addTime(temp2);
                    }
                }
            }
        }
        if (d.getType(to) == "EndEvent") {
            cout << "Left Multiple Path\n";
            return element;
        }
    }
    return NULL;
}


int main() {
    // Initialize the element counts
    int activity_count = 0;
    int gateway_count = 0;
    int xor_gateway_count = 0;
    int parallel_gateway_count = 0;
    int event_count = 0;
    int timer_event_count = 0;
    int message_event_count = 0;
    int start_event_count = 0;
    int end_event_count = 0;

    
    
    for (int i = 2; i <= 2; i++) {
        diagram diagramInfo;
        string type = "";
        string id = "";
        int time = 0;
        xml_document<> xpdl_doc;
        xml_node<>* root;
        file<> xpdl_file(i==2?"diagram2.xpdl":"diagram3.xpdl");
        // Parse the XPDL file
        xpdl_doc.parse<0>(xpdl_file.data());
        root = xpdl_doc.first_node("Package");
        std::cout << "\n\n------------- Diagram 0" << i << " ---------------\n";
        int j = 0;
        // Iterate over all the elements in the XPDL file and count the occurrences of each type of element
        for (xml_node<>* process = root->first_node("WorkflowProcesses")->first_node("WorkflowProcess"); process; process = process->next_sibling("WorkflowProcess")) {
            std::string process_name = process->name();
            xml_node<>* activities = process->first_node("Activities");
            if (activities) {
                for (xml_node<>* element = activities->first_node("Activity"); element; element = element->next_sibling()) {
                    time = 0;
                    id = element->first_attribute("Id") ? element->first_attribute("Id")->value() : "0";
                    xml_node<>* t = element->first_node("Description");
                    type = "Activity";
                    if (t!=NULL) {
                        string str = t->value();
                        if (str == "1") { time = 1; }
                        if (str == "2") { time = 2; }
                        if (str == "3") { time = 3; }
                        if (str == "4") { time = 4; }
                        if (str == "5") { time = 5; }
                        if (str == "6") { time = 6; }
                        if (str == "7") { time = 7; }
                        if (str == "8") { time = 8; }
                        if (str == "9") { time = 9; }
                        if (str == "10") { time = 10; }
                    }
                    activity_count++;
                    //cout << j << endl;
                    for (xml_node<>* events = element->first_node("Event"); events; events = events->next_sibling()) {
                        event_count++;
                        if (events->first_node("StartEvent")) { start_event_count++; type = "StartEvent"; activity_count--;}
                        if (events->first_node("EndEvent")) { end_event_count++; type = "EndEvent"; activity_count--; }
                    }
                    if (element->first_node("Implementation")) {
                        if (xml_node<>*task = element->first_node("Implementation")->first_node("Task")) {
                            if (task->first_node("TaskUser")) { type = "UserActivity"; }
                            if (task->first_node("TaskService")) { type = "ServiceActivity"; }
                            if (task->first_node("TaskRecieve")) { type = "RecieveActivity"; }
                            if (task->first_node("TaskSend")) { type = "SendActivity"; }
                            if (task->first_node("TaskScript")) { type = "ScriptActivity"; }
                            if (task->first_node("TaskManual")) { type = "ManualActivity"; }
                            if (task->first_node("TaskBusinessRule")) { type = "BusinessRuleActivity"; }
                        }
                    }
                    if (element->first_node("Route")) {
                        type = element->first_node("Route")->first_attribute("GatewayDirection")->value();
                        xor_gateway_count++;
                    }
                    diagramInfo.addActivity(type, id, time);
                  /*  cout << "Id: " << id << endl;
                    cout << "Type: " << type << endl;
                    cout << "Time: " << time << endl << endl;*/
                }

                // Display the element counts for the process model
                std::cout << "In the " << process_name << " process model, there are" << std::endl;
                std::cout << activity_count << " activities" << std::endl;
                std::cout << xor_gateway_count << " XOR gateways and " << parallel_gateway_count << " parallel gateways" << std::endl;
                std::cout << timer_event_count << " timer events, " << message_event_count << " message events, " << start_event_count << " start event and " << end_event_count << " end event" << std::endl;

            }
            
            cout << "done";
            // Updating Number of Outgoing Paths
            string from, to;
            xml_node<>* transitions = process->first_node("Transitions");
            if (transitions) {
                for (xml_node<>* element = transitions->first_node("Transition"); element; element = element->next_sibling()) {
                    from = element->first_attribute("From")->value();
                    diagramInfo.updateOutgoingPath(from);
                }
            }

            cout << "Transtions Initial Phase Found\n";
            
            transitions = process->first_node("Transitions");
            if (transitions) {
                int tmp = 0, temp2 = 0;
                for (xml_node<>* element = transitions->first_node("Transition"); element; element = element->next_sibling()) {
                    cout << "1" << endl;
                    from = element->first_attribute("From")->value();
                    to = element->first_attribute("To")->value();
                    if (diagramInfo.getPaths(from) > 1) {
                        cout << "Num Paths: " << diagramInfo.getPaths(from) << endl;
                        if (element->first_node("Condition")->first_node("Expression")) {
                            string str = element->first_node("Condition")->first_node("Expression")->value();
                            cout << "Value: " << str << endl;
                            element = element->next_sibling()->next_sibling()->next_sibling();
                            for (int i = diagramInfo.getPaths(from); i > 0; i--) {
                                float part = 0;
                                if (str == "0") { part = 0; }
                                if (str == "0.1") { part = 0.1; }
                                if (str == "0.2") { part = 0.2; }
                                if (str == "0.3") { part = 0.3; }
                                if (str == "0.4") { part = 0.4; }
                                if (str == "0.5") { part = 0.5; }
                                if (str == "0.6") { part = 0.6; }
                                if (str == "0.7") { part = 0.7; }
                                if (str == "0.8") { part = 0.8; }
                                if (str == "0.9") { part = 0.9; }
                                if (str == "1") { part = 1; }
                                element = multiplePath(element, part, diagramInfo);
                                element = element->previous_sibling();
                            }
                        }
                        else {
                            element = parallelPath(element->next_sibling(), diagramInfo, tmp);
                            if (tmp > temp2) {
                                temp2 = tmp;
                            }
                            if (i == 1) {
                                diagramInfo.addTime(temp2);
                            }
                        }
                        
                    }
                    diagramInfo.addTime(from);
                }
            }
            
            // Reset the element counts for the next process model
            activity_count = 0;
            gateway_count = 0;
            xor_gateway_count = 0;
            parallel_gateway_count = 0;
            event_count = 0;
            timer_event_count = 0;
            message_event_count = 0;
            start_event_count = 0;
            end_event_count = 0;
            break;
        }
        cout << "The Cycle Time of Process is: " << diagramInfo.getTime() << endl;
    }
    return 0;
}
