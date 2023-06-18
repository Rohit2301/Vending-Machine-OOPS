#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

class State
{
public:
    virtual void selectItem(string selection) = 0;
    virtual void insertMoney(int value) = 0;
    virtual void executeTransfer() = 0;
    virtual int cancelTransaction() = 0;
    virtual string tostring() = 0;
};

class AbstractState : public State
{
protected:
    VendingMachine *vendingmachine;

public:
    AbstractState() {}
    AbstractState(VendingMachine *vendingmachin)
    {
        this->vendingmachine = vendingmachin;
    }
};

class VendingMachine
{
private:
    string currentSelectedItem;
    int currentInsertedMoney;
    AbstractState *state;
    NoSelectionState *noSelectionState;
    map<string, int> itemPrice;

public:
    VendingMachine()
    {
        currentSelectedItem = "";
        currentInsertedMoney = 0;
        noSelectionState = new NoSelectionState(this);
        state = noSelectionState;

        itemPrice["Coke"] = 199;
        itemPrice["Fizz"] = 299;
        itemPrice["Mountain Dew"] = 399;
    }

    void emptySelectedItem()
    {
        this->currentSelectedItem = "";
    }

    void setSelectedItem(string item)
    {
        this->currentSelectedItem = item;
    }

    string getCurrentSelectedItem()
    {
        return this->currentSelectedItem;
    }

    void insertMoney(int money)
    {
        this->currentInsertedMoney += money;
    }

    int getCurrentInsertedMoney()
    {
        return this->currentInsertedMoney;
    }

    void setInsertedMoney(int money)
    {
        this->currentInsertedMoney = money;
    }

    int getSellPrice(string item)
    {
        if (item == "")
        {
            cout << "Select a item mf";
            return 0;
        }
        else
        {
            return itemPrice[item];
        }
    }

    // state

    void setState(AbstractState *state)
    {
        this->state = state;
    }
// /adding to increase no of commits
    // options for customer
    void selectItem(string selection)
    {
        state->selectItem(selection);
    }

    void addMoney(int value)
    {
        state->insertMoney(value);
    }

    void executeTransaction()
    {
        state->executeTransfer();
    }

    int cancelTransaction()
    {
        return state->cancelTransaction();
    }
};

class NoSelectionState : public AbstractState
{
public:
    NoSelectionState(VendingMachine *vendingmachine) : AbstractState(vendingmachine)
    {
    }
    NoSelectionState() {}

    void selectItem(string item) override
    {
        vendingmachine->setSelectedItem(item);
        vendingmachine->setState(new HasSelectionState(vendingmachine));
    }

    void insertMoney(int money) override
    {
        vendingmachine->setState(new InsertedMoneyState(vendingmachine));
    }

    void executeTransfer() override
    {
        cout << "You need to insert money first" << endl;
    }

    int cancelTransaction() override
    {
        cout << "Transaction cancelled";
        vendingmachine->setState(new NoSelectionState(vendingmachine));
        return 0;
    }

    string tostring() override
    {
        return "No selection";
    }
};

class HasSelectionState : public AbstractState
{
public:
    HasSelectionState() {}
    HasSelectionState(VendingMachine *vendingmachine) : AbstractState(vendingmachine)
    {
    }

    void selectItem(string item) override
    {
        vendingmachine->setSelectedItem(item);
    }

    void insertMoney(int money) override
    {
        vendingmachine->insertMoney(money);
    }

    void executeTransfer() override
    {
        cout << "Insert money first";
    }

    int cancelTransaction() override
    {
        cout << "Please select a item first" << endl;
        vendingmachine->setState(new NoSelectionState(vendingmachine));
        vendingmachine->emptySelectedItem();
        return 0;
    }

    string tostring() override
    {
        return "Has selection";
    }
};

class InsertedMoneyState : public AbstractState
{
public:
    InsertedMoneyState() {}
    InsertedMoneyState(VendingMachine *vendingmachine) : AbstractState(vendingmachine)
    {
    }

    void selectItem(string item) override
    {
        cout << "Item already selected mf";
    }

    void insertMoney(int money) override
    {
        vendingmachine->insertMoney(money);
    }

    void executeTransfer() override
    {
        int req = vendingmachine->getSellPrice(vendingmachine->getCurrentSelectedItem()) - vendingmachine->getCurrentInsertedMoney();
        if (req >= 0)
        {
            cout << "Take your soda";
            vendingmachine->setInsertedMoney(0);
            vendingmachine->setState(new NoSelectionState(vendingmachine));
            vendingmachine->setSelectedItem("");
        }
        else
        {
            cout << "Aur paise daal";
        }
    }

    int cancelTransaction() override
    {
        int insertmoney = vendingmachine->getCurrentInsertedMoney();
        cout << "Please select a item first" << endl;
        vendingmachine->setState(new NoSelectionState(vendingmachine));
        vendingmachine->emptySelectedItem();
        vendingmachine->setInsertedMoney(0);
        return insertmoney;
    }

    string tostring() override
    {
        return "Inserted Money";
    }
};

int main()
{

    VendingMachine *mach = new VendingMachine();
    system("cls");
    int itemNum;
    string item;
    do
    {
        cout << "Vending Machine Screen" << endl;
        cout << "Select item" << endl;
        cout << "1. Coke";
        cout << "2. Fizz";
        cout << "3. Mountain Dew";
        cin >> itemNum;
        switch (itemNum)
        {
        case 1:
            mach->selectItem("Coke");
            item = "Coke";
            break;
        case 2:
            mach->selectItem("Fizz");
            item = "Fizz";
            break;
        case 3:
            mach->selectItem("Mountain DEW");
            item = "Mountain DEW";
            break;

        default:
            break;
        }
        _getch();
        do
        {
            cout << "Insert" << mach->getCurrentSelectedItem() << " money for " << mach->getSellPrice(item) << endl;
            if (mach->getCurrentInsertedMoney() < mach->getSellPrice(item))
            {
                cout << "Insert req money";
            }
        } while (1);

    } while (1);

    return 0;
}