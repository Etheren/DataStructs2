//Student ID : 14040859

//Libraries used within the code
#include <string>			//String is used ONLY in the Product Descriptions
#include <iostream>			//IOStream is used to output Messages to the consoles 
#include <vector>			//Vector is used for states and state parameters only

using namespace std;		//Namespace is placed here to 

enum state { Out_Of_Poptart, No_Credit, Has_Credit, Dispenses_Poptart };//These are the states used in the dispenser depending on what the dispenser is doing,
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };		//And these are parameters used alongside the states

class StateContext;

//The state class is used for the different states
class State
{
protected:
	//This creates a reference from a class called StateContext, and calls it currentContext
	StateContext* CurrentContext;

public:

	State(StateContext* Context)
	{
		CurrentContext = Context;
	}
	virtual ~State(void) {}
	virtual void transition(void) {}
};


//StateContext is used to maintain the state objects we use
//Depending on the actions, our state object changes accordingly, for example, insterting poptarts when in the Out_Of_Poptarts state changes us to the No_Credit State
class StateContext
{
protected:

	State* CurrentState = nullptr;
	int stateIndex = 0;
	vector<State*> availableStates; //This is a vector used to store which state we will be using in our software
	vector<int> stateParameters;	//This is a vector used to store our state parameters,

public:
	virtual ~StateContext(void);	//This function is a Destructor used for the StateContext class
	virtual void setState(state newState);	//This function changes the state we are currently in to a different defined one, which is passed into the function
	virtual int getStateIndex(void);		//This function returns the value under stateIndex
	virtual void setStateParam(stateParameter SP, int value);	//This functions changes a state parameter. It requires which parameter to change, and what value to change it to
	virtual int getStateParam(stateParameter SP);		//This function returns the value that is stored under a defined state parameter, which is passed into the function
};

StateContext::~StateContext(void)		//The destructor for StateContext, to avoid us having memory leaks
{
	for (int i = 0; i < this->availableStates.size(); i++) delete this->availableStates[i];	
	this->availableStates.clear();		//This removes all our states we have stored,
	this->stateParameters.clear();		//and this removes the state parameters. Both these are functions called from the vector library
}

void StateContext::setState(state newState) //This function changes the state we are currently in to a different defined one, which is passed into the function
{
	this->CurrentState = availableStates[newState];
	this->stateIndex = newState;
	this->CurrentState->transition();	//This is a failsafe, that if a state is declared, and we cant move to it, an apporpriate error is outputted
}

int StateContext::getStateIndex(void)	//This returns the value under stateIndex.
{
	return this->stateIndex;
}

void StateContext::setStateParam(stateParameter SP, int value)	//This changes a defined state parameter with a defined value, for example
{																//We can set Credit to be equal to 50	
	this->stateParameters[SP] = value;
}

int StateContext::getStateParam(stateParameter SP)				//This returns the value stored under a defined state parameter
{																//Using the previous example, placing Credit in the function would return 50
	return this->stateParameters[SP];
}

class Transition	//This class handles the transitions between the different states, by default, each function, if not declared on each transition, returns an error message, and a false boolean result
{
public:
	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }			//These are virtual functions
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; }			//They are here that instead of the program crashing and returning a null value,
	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; }		//it instead returns an error, and a false ouput instead
	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; }			//We defined the correct transitions in each class later on,
	virtual bool dispense(void) { cout << "Error!" << endl; return false; }				//So we shouldnt see these errors pop up
};

class PoptartState : public State, public Transition				//This PoptartState class allows each poptart state to acces the error messages under the Transition class if not defined
{
public:
	PoptartState(StateContext* Context) : State(Context) {}
};

class OutOfPoptart : public PoptartState							//The OutOfPoptart, NoCredit, HasCredit and DispensesPoptart classes are used for the transitions between the different states	
{																	//Each class has the same functions as the Transition class, and depending on what state we need to transition to,
public:																//It will output a more detailed error message and return a false statement if there is no transition for that required statement,
	OutOfPoptart(StateContext* Context) : PoptartState(Context) {}	//Or return a detailed output, and return a true statement if a transition is needed.
	bool insertMoney(int money);									//The state we're in will decide which one of these representitive classes' functions are called, and therefore
	bool makeSelection(int option);									//The output of that function will change accordingly
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};
	
class NoCredit : public PoptartState								//the NoCredit state transition class
{
public:
	NoCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class HasCredit : public PoptartState								//the HasCredit state transition class
{
public:
	HasCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class DispensesPoptart : public PoptartState					//the DispensesPoptart state transition class
{
public:
	DispensesPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class Product									//The Product class contain information regarding each of the products, such as the cost and description for each product
{												
	friend class Filling;
protected:
	string product_description;					//This String value stores the name for the poptart
	int itemCost = 0;							//This interger value stores the cost of the poptart
public:
	virtual void consume(void)					//consume is the Destructor for the Product class
	{
		delete this;
	};
	virtual int cost(void)						//the Cost function simply returns the value that is stored under itemCost
	{
		return this->itemCost;
	}
	virtual string description(void)			//and description return the string value that is stored under product_description
	{
		return this->product_description;
	}
};

class Poptart : public Product							//the Poptart class is an example of the different products we have in our dispenser
{														//However, this is just an example, and is never used in our program
public:
	Poptart(void)
	{
		this->product_description = "Poptart";			//This sets the product description to be equal to "Poptart"
		this->itemCost = 50;							//Then this sets the price of it to be equal to 50.
	}
};

class Plain : public Product							//This is the class for the Plain Poptart, with the description and price
{
public:
	Plain(void) {
		this->itemCost = 100;
		this->product_description = "Plain Poptart";
	}
};

class Spicy : public Product							//This is the class for the Spicy Poptart, with the description and price
{
public:
	Spicy(void) {
		this->itemCost = 150;
		this->product_description = "Spicy Poptart";
	}
};

class Chocolate : public Product						//This is the class for the Chocolate Poptart, with the description and price
{
public:
	Chocolate(void) {
		this->itemCost = 200;
		this->product_description = "Chocolate Poptart";
	}
};

class Coconut : public Product							//This is the class for the Coconut Poptart, with the description and price
{
public:
	Coconut(void) {
		this->itemCost = 200;
		this->product_description = "Coconut Poptart";
	}
};

class Fruity : public Product							////This is the class for the Fruity Poptart, with the description and price
{
public:
	Fruity(void) {
		this->itemCost = 200;
		this->product_description = "Fruity Poptart";
	}
};

class Poptart_Dispenser : public StateContext, public Transition				//This is the main class for the Poptart Dispenser, implementing all the states
{																				
	friend class DispensesPoptart;	//this allows the DispensesPoptart state to be accessed through Poptart_Dispenser
	friend class HasCredit;			//This is also the same as above, but with the HasCredit state instead
private:
	PoptartState* PoptartCurrentState = nullptr;	
	bool itemDispensed = false;			//this indicates whether a product is there to be retrieved
	Product* DispensedItem = nullptr;	//Point to the final product
	bool itemRetrieved = false;			//this indicates whether a product has been retrieved
public:
	Poptart_Dispenser(int inventory_count);		//This is the constructor for the dispenser, asking for how many poptarts are in the dispenser to begin with
	~Poptart_Dispenser(void);					//This is destructor for the dispenser, used when the user stops a transaction, and to prevent memory leaks
	bool insertMoney(int money);				
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
	Product* getProduct(void);					//This function will check to see if the item has been dispensed.
	virtual void setStateParam(stateParameter SP, int value);		//This is the virtual declaration for the set state parameter function.
	virtual int getStateParam(stateParameter SP);					//This is the virtual declaration for the get state parameter function.
};

Poptart_Dispenser::~Poptart_Dispenser(void)			//This is the definition for the Poptart_Dispenser destructor, which is used if the purchase isnt completed, and the product is never dispensed
{
	if (!this->itemRetrieved)						//This is used if the user stops the transaction at any point, and removes the object from memory, to avoid a leak
	{
		delete this->DispensedItem;
	}
}

bool Poptart_Dispenser::insertMoney(int money)					//This will call insertMoney from the current state, and return the result of the function that has the money inserted into it
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;	//This line gets the current state from poptartState
	return this->PoptartCurrentState->insertMoney(money);
}

bool Poptart_Dispenser::makeSelection(int option)				//This calls makeSelection from the current state, and sets it to the option code that is chosen.
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;	// This line gets the current state from poptartState
	return this->PoptartCurrentState->makeSelection(option);
}

bool Poptart_Dispenser::moneyRejected(void)						//This function will call moneyRejected from the current state, and set it to reject money
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;	// This line gets the current state from poptartState
	return this->PoptartCurrentState->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)					//This function calls addPoptart from the current state, and set it to the value that is inputted
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;	//This line gets the current state from poptartState
	return this->PoptartCurrentState->addPoptart(number);
}

bool Poptart_Dispenser::dispense(void)							//This function calls dispense from the current state, and set it to dispense the item
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;	//This line gets the current state from poptartState
	return this->PoptartCurrentState->dispense();
}

Product* Poptart_Dispenser::getProduct(void)					//This function will check to see if the item has been dispensed
{																
	if (this->itemDispensed)			//If it has been dispensed
	{
		this->itemDispensed = false;	//set itemDispensed to be false, which then as a result, calls the destructor for the product.
		this->itemRetrieved = true;		//set the itemRetrieved to be true
		return this->DispensedItem;		//and return what the dispensed item
	}

	return nullptr;			//If there isnt a product to dispense, there would be an error, and the purchase is not completed yet.
}
void Poptart_Dispenser::setStateParam(stateParameter SP, int value)		//this sets the state parameter value to the Number of Poptarts, and the credit
{																		
	if (SP == Cost_Of_Poptart) return;
	this->stateParameters[SP] = value;
}

int Poptart_Dispenser::getStateParam(stateParameter SP)					//this returns the value of number of poptarts, and Credit.
{
	if (SP == Cost_Of_Poptart)						
	{
		if (DispensedItem == nullptr) return 0;
		return DispensedItem->cost();				//Since the cost of the poptart is defined in the product's cost() function, it calls that.
	}
	return this->stateParameters[SP];
}

bool OutOfPoptart::insertMoney(int money)			//If the user tries to insert money when there are no poptarts, output an error, and return false
{
	cout << "Out of Poptarts!" << endl;
	return false;
}

bool OutOfPoptart::makeSelection(int option)		//If the user tries to make a selection when there are no poptarts, output an error, and return false
{
	cout << "Out of Poptarts!" << endl;
	return false;
}
			
bool OutOfPoptart::moneyRejected(void)				//If the user tries to reject money when there are no poptarts, output a message, and return true
{
	cout << "Money has been returned" << endl;
	return true;
}

bool OutOfPoptart::addPoptart(int number)			//When poptarts are added to the dispenser, this outputs how many have been added, sets the No_Of_Poptarts parameter to the new inventory count
{													//Then change the state to No_Credit, and return true
	if (number == 1)
	{
		cout << number << " Poptart has been added" << endl;
	}
	else if (number > 1)
	{
		cout << number << " Poptarts have been added" << endl;
	}
	this->CurrentContext->setStateParam(No_Of_Poptarts, number);
	this->CurrentContext->setState(No_Credit);
	return true;
}

bool OutOfPoptart::dispense(void)					//If the dispenser attempts to dispense a poptart when out of poptarts, output an error and return false
{
	cout << "Out of Poptarts!" << endl;
	return false;
}

bool NoCredit::insertMoney(int money)					//If the user inserts money when the dispenser is out of money, output a message saying how much as added, change the Credit state parameter
{														//equal to the value that is inserted, then output a new message saying how much the user now has.
	cout << "You inserted: " << money << " Credits";	//Then change the current state to Has_Credit, and return true
	this->CurrentContext->setStateParam(Credit, money);
	cout << " Total: " << money << " Credits" << endl;
	this->CurrentContext->setState(Has_Credit);
	return true;
}

bool NoCredit::makeSelection(int option)			//If the user tries to make a selection, output an error message, and return false		
{
	cout << "No Money Inserted!" << endl;
	return false;
}
		
bool NoCredit::moneyRejected(void)					//If the dispenser tries to reject money, output an error message, and return false
{
	cout << "No Money Inserted!" << endl;
	return false;
}

bool NoCredit::addPoptart(int number)				//If a poptart is attempted to be added, output an error message, and return false
{
	cout << "Poptart already inserted" << endl;
	return false;
}

bool NoCredit::dispense(void)						//If the dispenser= tries to dispense a product, output an error message, and return false
{
	cout << "No Money Inserted" << endl;
	return false;
}

bool HasCredit::insertMoney(int money)							//If the user already has credit, and inserts more, this updates the Credit value with the old value and what was inserted together
{																//It also outputs a message with what was inserted, and the new total value
	cout << "You inserted: " << money << " Credits.";			//then it puts the state back into Has_Credit, and returns true
	this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit)+ money);
	cout << " Total: " << this->CurrentContext->getStateParam(Credit) << " Credits" << endl;
	this->CurrentContext->setState(Has_Credit);
	return true;
}

bool HasCredit::makeSelection(int option) {						//This function changes depending on what value is passed into the funciton
	switch (option) {
	case 1:
		((Poptart_Dispenser*)(this->CurrentContext))->DispensedItem = new Plain();	//Each case is double the previous case, and each case containse a different product to dispense.
		cout << "Plain Poptart Selected" << endl;
		break;

	case 2:
		((Poptart_Dispenser*)(this->CurrentContext))->DispensedItem = new Spicy();	//Each product sets the DispensedItem to be a new product. There is one from Plain, Spicy, Chocolate, Coconut and Fruity
		cout << "Spicy Poptart Selected" << endl;
		break;

	case 4:
		((Poptart_Dispenser*)(this->CurrentContext))->DispensedItem = new Chocolate();
		cout << "Chocolate Poptart Selected" << endl;
		break;
	
	case 8:
		((Poptart_Dispenser*)(this->CurrentContext))->DispensedItem = new Coconut();
		cout << "Coconut Poptart Selected" << endl;
		break;

	case 16:
		((Poptart_Dispenser*)(this->CurrentContext))->DispensedItem = new Fruity();
		cout << "Fruity Poptart Selected" << endl;
		break;

	default:
		cout << "No valid Poptart Selected" << endl;				//If the input doesnt match a correct case, it outputs an error message, and returns false
		return false;
		break;
	}

	if (this->CurrentContext->getStateParam(Credit) < this->CurrentContext->getStateParam(Cost_Of_Poptart))		//If the cost of the poptart is higher than the value stored under Credit, then the product is not dispensed.
	{
		delete ((Poptart_Dispenser*)(this->CurrentContext))->DispensedItem;
		cout << "Not enough credit inserted" << endl;
	}

	else
	{
													// If the user can afford the poptart however
	}
	{
		this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) - this->CurrentContext->getStateParam(Cost_Of_Poptart));	
		this->CurrentContext->setState(Dispenses_Poptart);		//Remove the cost of the poptart from the stored credit,
																//And set the state to be Dispenses_Poptart
	}
	return true;		//And return true
}

bool HasCredit::moneyRejected(void) {				//If the user wants their money to be back
	cout << "Money has been returned" << endl;		//Output a message to the screen
	this->CurrentContext->setStateParam(Credit, 0);	//Set the credit state parameter equal to be 0,
	this->CurrentContext->setState(No_Credit);		//Change the state to be No_Credit,
	return true;									//Then return true
}

bool HasCredit::addPoptart(int number)				//If a poptart is attempted to be added, output an error message, and return false
{
	cout << "Poptart already inserted" << endl;
	return false;
}

bool HasCredit::dispense(void) {					//If a poptart is attempted to be dispened without one being selected, output an error message, and return false
	cout << "Poptart must be selected before dispensing" << endl;
	return false;
}

bool DispensesPoptart::insertMoney(int money)		//If money is attempted to be inserted when a poptart is dispensing, output an error message, and return false
{
	cout << "Poptart Dispensing" << endl;
	return false;
}

bool DispensesPoptart::makeSelection(int option)	//If a selection when a poptart is dispensing, output an error message, and return false
{
	cout << "Poptart Dispensing" << endl;
	return false;
}

bool DispensesPoptart::moneyRejected(void)			//If money is attempted to be rejected when a poptart is dispensing, output an error message, and return false
{
	cout << "Poptart Dispensing" << endl;
	return false;
}

bool DispensesPoptart::addPoptart(int number)		//If poptarts are attempted to be added when a poptart is dispensing, output an error message, and return false
{
	cout << "Poptart Dispensing" << endl;
	return false;
}

bool DispensesPoptart::dispense(void)				//This is the funtion to control the dispensing item
{
	Product* dispensed = ((Poptart_Dispenser*)(CurrentContext))->DispensedItem;	//Declare a procuct called dispened, and set it to be equal to what the dispensed item is
	((Poptart_Dispenser*)(CurrentContext))->DispensedItem = nullptr; //This sets the Dispensed item to be equal to nullpointer, as the item has been dispensed
	dispensed->consume();		//call the consume function, which deletes the dispensed product from memory
	cout << "Dispensing Poptart" << endl;			//Output a message to the screen
	this->CurrentContext->setStateParam(No_Of_Poptarts, this->CurrentContext->getStateParam(No_Of_Poptarts) - 1); //lower the value that is stored under No_Of_Poptarts by 1)
	if (this->CurrentContext->getStateParam(No_Of_Poptarts) > 0)	//If there is at least more than 1 poptart left in the dispenser, then go to a check to see what the Credit is
	{
		if (this->CurrentContext->getStateParam(Credit) > 0)		//If there is at least 1 credit in the machine,
		{
			this->CurrentContext->setState(Has_Credit);				//Change the current state to Has_Credit
		}
		else if (this->CurrentContext->getStateParam(Credit) == 0)	//If there isn't any credit left in the machine
		{
			this->CurrentContext->setState(No_Credit);				//Change the current state to No_Credit
		}
	}
	else if (this->CurrentContext->getStateParam(No_Of_Poptarts) == 0)	//If the dispenser is out of poptarts,
	{
		cout << "Out of Poptarts. Money has been returned" << endl;		//Output a message to the screen
		this->CurrentContext->setStateParam(Credit, 0);					//Set the credit value to 0 to "return" the money
		this->CurrentContext->setState(Out_Of_Poptart);		//Change the current state to Out_Of_Poptart
	}
	return true;		//Then return true
}

Poptart_Dispenser::Poptart_Dispenser(int inventory_count)
{
															//This adds all the states and state parameters into the function as instances
	this->availableStates.push_back(new OutOfPoptart(this));//which allows them to be changed in the dispenser function
	this->availableStates.push_back(new NoCredit(this));	
	this->availableStates.push_back(new HasCredit(this));
	this->availableStates.push_back(new DispensesPoptart(this));
	this->stateParameters.push_back(No_Of_Poptarts);		//Number of Poptarts in the dispenser
	this->stateParameters.push_back(Credit);				 //Available Credit	in the dispenser
	this->stateParameters.push_back(Cost_Of_Poptart);
	
	this->setState(Out_Of_Poptart);				//The first state to start on is Out_Of_Poptarts
	if (inventory_count > 0)					//If the value that is passed into the function is 1 or greater,
	{
		this->addPoptart(inventory_count);		//Then add the number of poptarts thats been passed into the function into the dispenser, which then as a results, changes the state to No_Credit
	}
}