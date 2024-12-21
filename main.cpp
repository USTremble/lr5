#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

//абстрактный продукт
class HeroInterface {
public:
    virtual void displayCharacteristics() const = 0;
    virtual void attack(HeroInterface* target) = 0;
    virtual void takeDamage(int damage) = 0;
    virtual bool isAlive() const = 0;
    virtual ~HeroInterface() = default;
};

//базовый класс
class Hero : public HeroInterface {
protected:
    string name;
    string attribute;
    int moveSpeed;
    int health;
public:
    Hero(const string& name, const string& attribute, int moveSpeed, int health)
            : name(name), attribute(attribute), moveSpeed(moveSpeed), health(health) {}

    void takeDamage(int damage) override {
        srand(static_cast<unsigned>(time(0)));

        double modifier = (rand() % 51 - 25) / 100.0; // случайно в пределах +-0.2
        int modifiedDamage = static_cast<int>(damage * (1.0 + modifier));
        health -= modifiedDamage;

        if (health <= 0) {
            cout << name << " has fallen!\n" << endl;
        } else {
            cout << name << " remaining health: " << health << endl << endl;
        }

    }

    bool isAlive() const override {
        return health > 0;
    }

    virtual ~Hero() = default;
};

//продукт
class MageHero : public Hero {
private:
    int spellDamage;
public:
    MageHero(const string& name, const string& attribute, int moveSpeed, int health, int spellDamage)
            : Hero(name, attribute, moveSpeed, health), spellDamage(spellDamage) {}

    void displayCharacteristics() const override {
        cout << "Mage Hero: " << name << " (" << attribute << ")" << ", Speed: " << moveSpeed
             << ", Health: " << health << ", Spell Damage: " << spellDamage << endl;
    }

    void attack(HeroInterface* target) override {
        cout << name << " casts a spell on the enemy!" << endl;
        target->takeDamage(spellDamage);
    }
};

class MeleeHero : public Hero {
private:
    int meleeDamage;
public:
    MeleeHero(const string& name, const string& attribute, int moveSpeed, int health, int meleeDamage)
            : Hero(name, attribute, moveSpeed, health), meleeDamage(meleeDamage) {}

    void displayCharacteristics() const override {
        cout << "Melee Hero: " << name << " (" << attribute << ")" << ", Speed: " << moveSpeed
             << ", Health: " << health << ", Melee Damage: " << meleeDamage << endl;
    }

    void attack(HeroInterface* target) override {
        cout << name << " strikes the enemy!" << endl;
        target->takeDamage(meleeDamage);
    }
};

class RangeHero : public Hero {
private:
    int range;
public:
    RangeHero(const string& name, const string& attribute, int moveSpeed, int health, int range)
            : Hero(name, attribute, moveSpeed, health), range(range) {}

    void displayCharacteristics() const override {
        cout << "Range Hero: " << name << " (" << attribute << ")" << ", Speed: " << moveSpeed
             << ", Health: " << health << ", Attack Range: " << range << endl;
    }

    void attack(HeroInterface* target) override {
        int damage = range / 10;
        cout << name << " shoots the enemy!" << endl;
        target->takeDamage(damage);
    }
};

//сама абстрактная фабрика
class CharacterFactory {
public:
    virtual MageHero* createMage(const string& name, const string& attribute, int moveSpeed, int health, int spellDamage) const = 0;
    virtual MeleeHero* createMelee(const string& name, const string& attribute, int moveSpeed, int health, int meleeDamage) const = 0;
    virtual RangeHero* createRange(const string& name, const string& attribute, int moveSpeed, int health, int range) const = 0;
    virtual ~CharacterFactory() = default;
};

//конкретная фабрики
class RadiantHeroFactory : public CharacterFactory {
public:
    MageHero* createMage(const string& name, const string& attribute, int moveSpeed, int health, int spellDamage) const override {
        return new MageHero(name, attribute, moveSpeed, health, spellDamage);
    }

    MeleeHero* createMelee(const string& name, const string& attribute, int moveSpeed, int health, int meleeDamage) const override {
        return new MeleeHero(name, attribute, moveSpeed, health, meleeDamage);
    }

    RangeHero* createRange(const string& name, const string& attribute, int moveSpeed, int health, int range) const override {
        return new RangeHero(name, attribute, moveSpeed, health, range);
    }
};

class DireHeroFactory : public CharacterFactory {
public:
    MageHero* createMage(const string& name, const string& attribute, int moveSpeed, int health, int spellDamage) const override {
        return new MageHero(name, attribute, moveSpeed, health, spellDamage);
    }

    MeleeHero* createMelee(const string& name, const string& attribute, int moveSpeed, int health, int meleeDamage) const override {
        return new MeleeHero(name, attribute, moveSpeed, health, meleeDamage);
    }

    RangeHero* createRange(const string& name, const string& attribute, int moveSpeed, int health, int range) const override {
        return new RangeHero(name, attribute, moveSpeed, health, range);
    }
};

void battle(vector<HeroInterface*>& radiantTeam, vector<HeroInterface*>& direTeam) {
    srand(static_cast<unsigned>(time(0)));

    while (!radiantTeam.empty() && !direTeam.empty()) {

        int rIndex = rand() % radiantTeam.size();
        int dIndex = rand() % direTeam.size();

        HeroInterface* hero1 = radiantTeam[rIndex];
        HeroInterface* hero2 = direTeam[dIndex];

        hero1->attack(hero2);
        if (!hero2->isAlive()) {
            delete direTeam[dIndex];
            direTeam.erase(direTeam.begin() + dIndex);
            continue;
        }

        hero2->attack(hero1);
        if (!hero1->isAlive()) {
            delete radiantTeam[rIndex];
            radiantTeam.erase(radiantTeam.begin() + rIndex);
        }
    }

    if (radiantTeam.empty()) {
        cout << "Dire wins the game! \nGG" << endl;
    } else {
        cout << "Radiant wins the game! \nGG" << endl;
    }
}

int main() {
    RadiantHeroFactory radiantFactory;
    DireHeroFactory direFactory;

    vector<HeroInterface*> radiantTeam = {
            radiantFactory.createMage("Invoker", "Intelligence", 310, 80, 60),
            radiantFactory.createMelee("Phantom Assassin", "Agility", 290, 10, 50),
            //radiantFactory.createRange("Sniper", "Agility", 300, 80, 550)
    };

    vector<HeroInterface*> direTeam = {
            direFactory.createMage("Zeus", "Intelligence", 305, 110, 40),
            direFactory.createRange("Drow Ranger", "Agility", 310, 80, 550),
            //direFactory.createMelee("Sven", "Strength", 295, 120, 55)
    };

    cout << "Radiant Team:\n";
    for (const auto& hero : radiantTeam) {
        hero->displayCharacteristics();
    }

    cout << "\nDire Team:\n";
    for (const auto& hero : direTeam) {
        hero->displayCharacteristics();
    }

    cout << "\nBattle begins!\n";
    battle(radiantTeam, direTeam);

    for (auto& hero : radiantTeam) {
        delete hero;
    }
    for (auto& hero : direTeam) {
        delete hero;
    }

    return 0;
}
