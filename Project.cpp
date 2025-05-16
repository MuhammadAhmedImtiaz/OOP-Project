#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_MOBILES = 100;

class Mobile {
protected:
    string brand, model;
    double price;

public:
    Mobile() : brand(""), model(""), price(0) {}

    Mobile(string b, string m, double p) {
        brand = b;
        model = m;
        setPrice(p);
    }

    virtual void display() {
        cout << "Brand: " << brand << ", Model: " << model << ", Price: " << price << endl;
    }

    string getModel() {
        return model;
    }

    void setPrice(double p) {
        if (p < 0) throw "Price cannot be negative.";
        price = p;
    }

    virtual void read(ifstream &in) {
        in >> brand >> model >> price;
    }

    virtual void write(ofstream &out) {
        out << brand << " " << model << " " << price << endl;
    }
};

class SimpleMobile : public Mobile {
public:
    SimpleMobile() {}
    SimpleMobile(string b, string m, double p) : Mobile(b, m, p) {}

    void display() override {
        cout << "Simple Mobile -> ";
        Mobile::display();
    }
};

class SmartPhone : public Mobile {
    string os;

public:
    SmartPhone() : os("") {}
    SmartPhone(string b, string m, double p, string o) : Mobile(b, m, p), os(o) {}

    void display() override {
        cout << "Smartphone -> ";
        Mobile::display();
        cout << "Operating System: " << os << endl;
    }

    void read(ifstream &in) override {
        Mobile::read(in);
        in >> os;
    }

    void write(ofstream &out) override {
        Mobile::write(out);
        out << os << endl;
    }
};

void addSimple(Mobile* list[], int &count) {
    string b, m;
    double p;
    cout << "Enter brand, model, price: ";
    cin >> b >> m >> p;
    try {
        list[count++] = new SimpleMobile(b, m, p);
    } catch (const char* msg) {
        cout << msg << endl;
    }
}

void addSmart(Mobile* list[], int &count) {
    string b, m, o;
    double p;
    cout << "Enter brand, model, price, OS: ";
    cin >> b >> m >> p >> o;
    try {
        list[count++] = new SmartPhone(b, m, p, o);
    } catch (const char* msg) {
        cout << msg << endl;
    }
}

void displayAll(Mobile* list[], int count) {
    for (int i = 0; i < count; i++) {
        list[i]->display();
    }
}

void updatePrice(Mobile* list[], int count) {
    string m;
    double p;
    cout << "Enter model to update: ";
    cin >> m;
    for (int i = 0; i < count; i++) {
        if (list[i]->getModel() == m) {
            cout << "Enter new price: ";
            cin >> p;
            try {
                list[i]->setPrice(p);
                cout << "Price updated.\n";
            } catch (const char* msg) {
                cout << msg << endl;
            }
            return;
        }
    }
    cout << "Model not found.\n";
}

void searchMobile(Mobile* list[], int count) {
    string m;
    cout << "Enter model to search: ";
    cin >> m;
    for (int i = 0; i < count; i++) {
        if (list[i]->getModel() == m) {
            list[i]->display();
            return;
        }
    }
    cout << "Model not found.\n";
}

void deleteMobile(Mobile* list[], int &count) {
    string m;
    cout << "Enter model to delete: ";
    cin >> m;
    for (int i = 0; i < count; i++) {
        if (list[i]->getModel() == m) {
            delete list[i];
            for (int j = i; j < count - 1; j++)
                list[j] = list[j + 1];
            count--;
            cout << "Deleted.\n";
            return;
        }
    }
    cout << "Model not found.\n";
}

void saveToFile(Mobile* list[], int count) {
    ofstream out("mobiles.txt");
    for (int i = 0; i < count; i++) {
        if (dynamic_cast<SimpleMobile*>(list[i]))
            out << "Simple ";
        else
            out << "Smart ";
        list[i]->write(out);
    }
    out.close();
    cout << "Saved to file.\n";
}

void loadFromFile(Mobile* list[], int &count) {
    ifstream in("mobiles.txt");
    string type;
    while (in >> type && count < MAX_MOBILES) {
        if (type == "Simple") {
            SimpleMobile* s = new SimpleMobile();
            s->read(in);
            list[count++] = s;
        } else if (type == "Smart") {
            SmartPhone* s = new SmartPhone();
            s->read(in);
            list[count++] = s;
        }
    }
    in.close();
    cout << "Loaded from file.\n";
}

void menu() {
    cout << "\n1. Add Simple Mobile\n2. Add Smartphone\n3. Display All\n4. Update Price\n5. Search Mobile\n6. Delete Mobile\n7. Save to File\n8. Load from File\n9. Exit\n";
}

int main() {
    Mobile* list[MAX_MOBILES];
    int count = 0, choice;

    do {
        menu();
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: addSimple(list, count); break;
            case 2: addSmart(list, count); break;
            case 3: displayAll(list, count); break;
            case 4: updatePrice(list, count); break;
            case 5: searchMobile(list, count); break;
            case 6: deleteMobile(list, count); break;
            case 7: saveToFile(list, count); break;
            case 8: loadFromFile(list, count); break;
            case 9: cout << "Exiting...\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 9);

    // Free memory
    for (int i = 0; i < count; i++)
        delete list[i];

    return 0;
}
