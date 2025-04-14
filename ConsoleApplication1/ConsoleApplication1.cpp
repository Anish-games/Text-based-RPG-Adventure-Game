#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;

// Base class for all characters in the game
class Chara
{
protected:
    int Health; // Health of the character
    int attack; // Attack power of the character
public:
    virtual ~Chara() {} // Virtual destructor
    virtual void TakeDamage(int attack) = 0;
    int gethealth() { return Health; }
    virtual void GiveDamage(Chara* player) = 0;
    bool isalive() { return(Health > 0); }
};

// Player class derived from Chara
class Player : public Chara
{
private:
    int Maxhealth;   int attack;// Maximum health of the player
    //int attack = 20; // Attack power of the player
    int Healing, MaxHealing, MiniHealing;
    bool hasSword = false; //  to check if player has a sword
    bool hasShield = false; // to check if player has a shield
    bool hasArmour = false; //  to check if player has an armour
    bool hasBow = false; //  to check if player has a bow

public:

    Player() {
        Maxhealth = 100; // Maximum health of the player
        attack = 20;
        MaxHealing = 45; MiniHealing = 25;
        Health = Maxhealth; // Initialize Health
    }

    // Function to take damage
    void TakeDamage(int attack) override
    {
        if (hasShield && (rand() % 10 == 0)) {
            cout << "Blocker activated! Malhar takes no damage.-----------\n";
            return;
        }
        Health -= attack;
        cout << "\n" << "Malhar's current health is " << Health << " after receiving damage of " << attack << ".\n" << endl;
    }

    // Function to give damage
    void GiveDamage(Chara* player) override
    {
        int damage = attack;
        if (shouldUseSpecialAbility()) {
            if (hasSword) {
                cout << "Critical hit activated!-----------\n";
                damage *= 2;
            }
            if (hasBow) {
                cout << "Ranged Attack activated! Malhar won't take damage for the next attack.\n";

            }
        }
        cout << "\n" << "Malhar is giving damage to enemy. The damage is " << damage << ".\n" << endl;
        player->TakeDamage(damage);
        if (hasArmour && (rand() % 10 == 0)) {
            cout << "Life steal activated!\n";
            Health += damage / 10;
        }
    }

    // Function to heal the player
    void heal()
    {
        Healing = (rand() % (MaxHealing - MiniHealing + 1)) + MiniHealing;
        Health += Healing;
        if (Health > Maxhealth)
        {
            Health = Maxhealth;
            cout << "\n" << "Malhar got " << Healing << " healing,Malhar has 100% Full Health now.\n" << endl;
        }
        else
        {
            cout << "\n" << "Malhar got healing of " << Healing << ". Now, his health is " << Health << ".\n" << endl;
        }
    }

    // Function to unlock the sword
    void unlockSword() {
        hasSword = true;
        cout << "New Special Ability Awarded, Sword unlocked!\n";
        cout << "                                             \n";
    }

    // Function to unlock the shield
    void unlockShield() {
        hasShield = true;
        cout << "New Special Ability Awarded, Shield unlocked!\n";
        cout << "                                             \n";
    }

    // Function to unlock the armour
    void unlockArmour() {
        hasArmour = true;
        cout << "New Special Ability Awarded, Armour unlocked!\n";
        cout << "                                             \n";
    }

    // Function to unlock the bow
    void unlockBow() {
        hasBow = true;
        cout << "New Special Ability Awarded, Bow unlocked!\n";
        cout << "                                             \n";
    }

    // Function to level up the player
    void levelUp() {
        int oldMaxHealth = Maxhealth;
        int oldAttack = attack;

        Maxhealth += 10; // Increase Max HP
        Health = Maxhealth; // Reset the health to max
        attack += 5; // Increase Melee Damage

        cout << "Malhar leveled up! Max HP increased by " << (Maxhealth - oldMaxHealth)
            << " and Melee Damage increased by " << (attack - oldAttack) << ".\n";
    }

    // Function to check if special ability should be used
    bool shouldUseSpecialAbility() {
        return Health <= 0.2 * Maxhealth || (rand() % 10 == 0);
    }

    // Function to get the attack power of the player
    int getattack() { return attack; }
    ~Player() {}
};


// Enemy class derived from Chara
class Enemy : public Chara
{
protected:
    int attack = 10;
    int id; // Add an id field to identify the enemy

public:


    // Constructor to initialize the enemy
    Enemy(int id) {
        this->id = id;
        Health = 100;
    }

    // Function to take damage
    void TakeDamage(int attack) override
    {
        Health -= attack;
        cout << "\n" << "Enemy " << id << " current health is " << Health << " after receiving damage of " << attack << ".\n" << endl;
    }

    // Function to give damage
    void GiveDamage(Chara* player) override
    {
        cout << "\n" << "Enemy " << id << " is attacking. The damage is " << attack << ".\n" << endl;
        player->TakeDamage(attack);
    }

};

// Boss class derived from Enemy
class Boss : public Enemy
{
public:
    // Constructor to initialize the boss
    Boss(int id) : Enemy(id) {
        Health = 300;
        attack = 30;
    }

    // Ground Slash ability: does double damage
    void GroundSlash(Chara* player) {
        int damage = attack * 2;
        cout << "Murloc uses Ground Slash! It deals " << damage << " damage.\n";
        player->TakeDamage(damage);
    }

    // Speed Dash ability: does normal damage
    void SpeedDash(Chara* player) {
        cout << "Murloc uses Speed Dash! It deals " << attack << " damage.\n";
        player->TakeDamage(attack);
    }

    // Regenerate ability: regains health
    void Regenerate() {
        cout << "Murloc uses Health Regeneration! It regains 10 health.\n";
        Health += 10;
        if (Health > 200) Health = 200; // Cap health at 200
    }

    // Function to give damage
    void GiveDamage(Chara* player) override {
        int ability = rand() % 3;
        switch (ability) {
        case 0: GroundSlash(player); break;
        case 1: SpeedDash(player); break;
        case 2: Regenerate(); break;
        }
    }
};

// Class for the game loop
class GameLoop {
public:
    shared_ptr<Player> playerone;

    GameLoop() : playerone(make_shared<Player>()) {}

    void printWelcomeMessage() {
        cout << "Welcome to the Text-based RPG Adventure Game!\n";
        cout << "                                             \n";
        cout << "In a world filled with monsters and danger, our hero Malhar stands tall.\n";
        cout << "                                             \n";
        cout << "His village has been captured by a monster named Murlocs, and it's up to him to save it.\n";
        cout << "                                             \n";
        cout << "With courage in his heart and a sword in his hand, Malhar embarks on his journey.\n";
        cout << "                                             \n";
        cout << "Malhar's current stats are:\n";
        cout << "                                             \n";
        cout << "Health: " << playerone->gethealth() << "\n";
        cout << "Attack: " << playerone->getattack() << "\n";
        cout << "                                             \n";
        cout << "Get ready to join Malhar on his epic adventure!\n";
    }

    void gameLoop() {
        for (int level = 1; level <= 6; level++) {
            cout << "\nLevel " << level << " begins now.\n";

            vector<Enemy*> enemies;
            if (level == 6) {
                enemies.push_back(new Boss(1));
            }
            else {
                for (int i = 0; i < level; i++) {
                    enemies.push_back(new Enemy(i + 1));
                }
            }

            while (playerone->isalive() && !enemies.empty()) {
                cout << "__________________________" << endl;
                cout << "\n" << "Press A to attack or press H to heal.\n" << endl;
                char Input;
                cin >> Input;
                if (Input == 'A' || Input == 'a') {
                    for (auto enemy : enemies) {
                        playerone->GiveDamage(enemy);
                        if (!enemy->isalive()) {
                            enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
                            delete enemy;
                            break;
                        }
                    }
                }
                else if (Input == 'H' || Input == 'h') {
                    playerone->heal();
                }
                if (enemies.empty()) {
                    cout << "\n" << "All enemies died, Malhar won!\n" << endl;
                    break;
                }
                if (playerone->isalive()) {
                    for (auto enemy : enemies) {
                        enemy->GiveDamage(playerone.get());
                        if (!playerone->isalive()) {
                            break;
                        }
                    }
                }
                if (!playerone->isalive()) {
                    cout << "\n" << "Malhar died, enemies won.\n" << endl;
                    break;
                }
            }

            for (auto enemy : enemies) {
                delete enemy;
            }
            enemies.clear();

            if (!playerone->isalive()) {
                break;
            }

            // Unlock special abilities after each level
            switch (level) {
            case 2: playerone->unlockSword(); break;
            case 3: playerone->unlockShield(); break;
            case 4: playerone->unlockArmour(); break;
            case 5: playerone->unlockBow(); break;
            }
            // Level up after each level
            if (level < 6) { // Add this condition
                playerone->levelUp(); // Level up the player
                cout << "Health reset. Malhar's new health is " << playerone->gethealth() << ".\n"; // Display the new health
            }
        }
        // Check if the player is alive
        if (playerone->isalive()) {
            cout << "\nMalhar has defeated Murloc and saved his village!\n";
            cout << "Thank you for playing the Text-based RPG Adventure Game. Hope you enjoyed the adventure!\n";
        }
        else {
            cout << "\nMalhar has fallen in battle. The village remains under Murloc's control.\n";
            cout << "Game Over. Better luck next time!\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator
    GameLoop game;
    game.printWelcomeMessage();
    game.gameLoop();
    return 0;
}