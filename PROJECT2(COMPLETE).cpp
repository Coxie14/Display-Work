#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> avenues;
vector<string> streets;

vector<string> loadFromFile(const string& fileName) {
  // set up a file stream and open it for reading***********
  string hold;
  // Create a new vector of strings
  vector<string> result;
  fstream file1(fileName, ios::in);

  if (file1) {
    while (getline(file1, hold, '\n')) {
      if (!(hold.empty())) {
        result.push_back(hold);
      }
    }
  }
  file1.close();
  return result;
}
enum Direction { NORTH, EAST, SOUTH, WEST };

ostream& operator<<(ostream& os, const Direction& dir) {
  switch (dir) {
    case NORTH:
      os << "NORTH";
      break;
    case EAST:
      os << "EAST";
      break;
    case SOUTH:
      os << "SOUTH";
      break;
    case WEST:
      os << "WEST";
      break;
  }
  return os;
}

class Intersection {
 private:
  string streetName;
  string avenueName;
  Intersection* northNeighbor;
  Intersection* eastNeighbor;
  Intersection* southNeighbor;
  Intersection* westNeighbor;
  bool isBusStop;

 public:
  Intersection(string sName, string aName) {
    this->streetName = sName;
    this->avenueName = aName;
    this->northNeighbor = nullptr;
    this->eastNeighbor = nullptr;
    this->southNeighbor = nullptr;
    this->westNeighbor = nullptr;
  }
  Intersection() {
    this->streetName = "";
    this->avenueName = "";
    this->northNeighbor = nullptr;
    this->eastNeighbor = nullptr;
    this->southNeighbor = nullptr;
    this->westNeighbor = nullptr;
  }
  // Accessors
  string getStreetName() const { return this->streetName; }
  string getAvenueName() const { return this->avenueName; }
  Intersection* getNorthNeighbor() const { return this->northNeighbor; }
  Intersection* getEastNeighbor() const { return this->eastNeighbor; }
  Intersection* getSouthNeighbor() const { return this->southNeighbor; }
  Intersection* getWestNeighbor() const { return this->westNeighbor; }
  bool getIsBusStop() const { return isBusStop; }

  // Mutators
  void setNorthNeighbor(Intersection* x) {
    this->northNeighbor = x;
    if (x != nullptr) {
      x->southNeighbor = this;
    }
  }
  void setEastNeighbor(Intersection* y) {
    this->eastNeighbor = y;
    if (y != nullptr) {
      y->westNeighbor = this;
    }
  }
  void setSouthNeighbor(Intersection* b) {
    this->southNeighbor = b;
    if (b != nullptr) {
      b->northNeighbor = this;
    }
  }
  void setWestNeighbor(Intersection* w) {
    this->westNeighbor = w;
    if (w != nullptr) {
      w->eastNeighbor = this;
    }
  }

  void setIsBusStop(bool value) { isBusStop = value; }
};

class Vehicle {
 protected:
  string name;
  Intersection* location;
  Direction direction;

 public:
  Vehicle(string name, Intersection* location, Direction direction)
      : name(name), location(location), direction(direction) {}

  // Accessors and Mutators
  string getName() const { return name; }
  Intersection* getLocation() const { return location; }
  Direction getDirection() const { return direction; }

  void setLocation(Intersection* loc) { location = loc; }
  void setDirection(Direction dir) { direction = dir; }

  // Member functions
  void turnRight() {
    switch (direction) {
      case NORTH:
        direction = EAST;
        break;
      case EAST:
        direction = SOUTH;
        break;
      case SOUTH:
        direction = WEST;
        break;
      case WEST:
        direction = NORTH;
        break;
    }
  }

  void turnLeft() {
    switch (direction) {
      case NORTH:
        direction = WEST;
        break;
      case EAST:
        direction = NORTH;
        break;
      case SOUTH:
        direction = EAST;
        break;
      case WEST:
        direction = SOUTH;
        break;
    }
  }

  virtual bool canStopHere(Intersection* intersection) const {
    return true;  // Default behavior, overridden in derived classes
  }

  bool drive() {
    switch (direction) {
      case NORTH:
        if (location->getNorthNeighbor()) {
          location = location->getNorthNeighbor();
          return true;
        }
        break;
      case EAST:
        if (location->getEastNeighbor()) {
          location = location->getEastNeighbor();
          return true;
        }
        break;
      case SOUTH:
        if (location->getSouthNeighbor()) {
          location = location->getSouthNeighbor();
          return true;
        }
        break;
      case WEST:
        if (location->getWestNeighbor()) {
          location = location->getWestNeighbor();
          return true;
        }
        break;
    }
    return false;
  }
};

class Car : public Vehicle {
 public:
  Car(string name, Intersection* location, Direction direction)
      : Vehicle(name, location, direction) {}

  // Override canStopHere to disallow stopping at bus stops
  bool canStopHere(Intersection* intersection) const override {
    return !intersection->getIsBusStop();
  }
};

class Bus : public Vehicle {
 public:
  Bus(string name, Intersection* location, Direction direction)
      : Vehicle(name, location, direction) {}

  // Override canStopHere to only allow stopping at bus stops
  bool canStopHere(Intersection* intersection) const override {
    return intersection->getIsBusStop();
  }
};

//
void grandTour(Vehicle& vehicle, Intersection* startingIntersection) {
  // Start at the top left intersection
  Intersection* currentIntersection = startingIntersection;

  // Drive down one street, then west through the next street, and repeat
  Direction currentDirection = EAST;  // Start by going east
  cout << "Driving on " << currentIntersection->getStreetName() << " & "
           << currentIntersection->getAvenueName() << endl;

  if (vehicle.canStopHere(currentIntersection)) {
      cout << "Vehicle is stopping at intersection\n";
  }

  while (currentIntersection) {
    // Drive down the street
    while (currentIntersection->getEastNeighbor() != nullptr) {
      currentIntersection = currentIntersection->getEastNeighbor();
      cout << "Driving on " << currentIntersection->getStreetName() << " & "
           << currentIntersection->getAvenueName() << endl;

      // Check if the vehicle should stop at this intersection
     if (vehicle.canStopHere(currentIntersection)) {
        cout << "Stopping\n";
        // Perform stopping actions here
      }
      else{
        cout << "Can't Stop Here\n";
      }
   }
 
    if (currentIntersection->getSouthNeighbor() == nullptr){
      cout << "End of Tour";
      return;
     }
     else {
     currentIntersection = currentIntersection->getSouthNeighbor();
       cout << "Driving on " << currentIntersection->getStreetName() << " & " << currentIntersection->getAvenueName() << endl;
     if (vehicle.canStopHere(currentIntersection)) {
        cout << "Stopping\n";
        // Perform stopping actions here
      }
      else{
        cout << "Can't Stop Here\n";
      }
     }

 while (currentIntersection->getWestNeighbor() != nullptr) {
      currentIntersection = currentIntersection->getWestNeighbor();
      cout << "Driving on " << currentIntersection->getStreetName() << " & "
           << currentIntersection->getAvenueName() << endl;

      // Check if the vehicle should stop at this intersection
       if (vehicle.canStopHere(currentIntersection)) {
        cout << "Stopping\n";
        // Perform stopping actions here
      }
      else{
        cout << "Can't Stop Here\n";
      }
   }
     if (currentIntersection->getSouthNeighbor() == nullptr){
      cout << "End of Tour";
      return;
     }
     else {
     currentIntersection = currentIntersection->getSouthNeighbor();
       cout << "Driving on " << currentIntersection->getStreetName() << " & " << currentIntersection->getAvenueName() << endl;
      if (vehicle.canStopHere(currentIntersection)) {
        cout << "Stopping\n";
        // Perform stopping actions here
      }
      else{
        cout << "Can't Stop Here\n";
      }
     }
    }
}


Intersection* buildGridville(vector<string> streets, vector<string> avenues) {
  Intersection* topLeft = nullptr;
  vector<Intersection*> prevRow(avenues.size(), nullptr);

  for (int s = 0; s < streets.size(); s++) {
    Intersection* previous = nullptr;

    for (int a = 0; a < avenues.size(); a++) {
      Intersection* intx = new Intersection(streets[s], avenues[a]);
      intx->setNorthNeighbor(previous);

      if (prevRow[a] != nullptr) {
        intx->setWestNeighbor(prevRow[a]);
      }

      prevRow[a] = intx;
      previous = intx;

      if (s == 0 && a == 0) {
        topLeft = intx;
      }

      // 9% chance of being a bus stop
      if (rand() % 100 < 9) {
        intx->setIsBusStop(true);
      }
    }
  }
  return topLeft;
}

int main() {
  streets = loadFromFile("streets.txt");
  avenues = loadFromFile("avenues.txt");

  // Build Gridville using the vector
  Intersection *start = buildGridville(streets, avenues);

  // create cars to travel with
  Vehicle car("generic", start, EAST);
  Car corvette("CORV", start, EAST);
  // Bus hound;
   Bus hound("", start, EAST);
  // go on tour
  cout << "Generic Vehicle:" << endl;
  grandTour(car, start);
  cout << "\n\nCAR:" << endl;
  grandTour(corvette, start);
  cout << "\n\nBUS:" << endl;
  grandTour(hound, start);
  return 0;
}