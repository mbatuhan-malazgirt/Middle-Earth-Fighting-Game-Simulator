#include "Character.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

static bool is_it_over(Character**& one, Character**& two, int count, bool draw) // controlling is war over or not.
{
    int deaths = 0;
    if (draw) // draw scenario
    {
        return true;
    }
    if (count % 2 == 0) // community 1 won.
    {
        for (int i = 0; i < 5; i++)
        {
            if (two[i]->type == "Hobbit" && !two[i]->isAlive) // if hobbit is dead
            {
                return true;
            }
            if (!two[i]->isAlive) // counting casulties.
            {
                deaths++;
            }
        }
        if (deaths == 4)
        {
            return true;
        }
        return false;
    }
    else // community 2 won.
    {
        for (int i = 0; i < 5; i++)
        {
            if (one[i]->type == "Hobbit" && !one[i]->isAlive)
            {
                return true;
            }
            if (!one[i]->isAlive)
            {
                deaths++;
            }
        }
        if (deaths == 4)
        {
            return true;
        }
        return false;
    }
}

static void update_communities(Character**& x, Character**& y, int count) // updating community info every raund.
{
    for (int q = 0; q < 5; q++)
    {
        x[q]->nRoundsSinceSpecial += 1;
        y[q]->nRoundsSinceSpecial += 1;
        x[q]->healthHistory[count - 11] = x[q]->remainingHealth;
        y[q]->healthHistory[count - 11] = y[q]->remainingHealth;
    }
}

static void attack(Character& defender, Character& attacker, int& last_killed_one, int& last_killed_two, int defender_id, int attacker_id, int count) // one character attacks another. this function also checks last killed character for each community.
{
    int damage = attacker.attack - defender.defense;
    if (damage > 0)
    {
        defender.remainingHealth = defender.remainingHealth - damage;
        if (defender.remainingHealth <= 0)
        {
            defender.remainingHealth = 0;
            defender.isAlive = false;

            if (count % 2 == 0)
            {
                last_killed_two = defender_id;
            }
            else if (count % 2 == 1)
            {
                last_killed_one = defender_id;
            }

        }

    }

}

static void alphabetic_order(Character**& all_characters_community_one, Character**& all_characters_community_two) // sorting character in terms of alphabetic order.
{
    for (int p = 0; p < 5; p++) // bubble sort algorithm.
    {
        for (int j = 0; j < 4 - p; j++)
        {
            if ((all_characters_community_one[j]->name).compare(all_characters_community_one[j + 1]->name) > 0)
            {
                Character* temp = all_characters_community_one[j];               
                all_characters_community_one[j] = all_characters_community_one[j + 1];
                all_characters_community_one[j + 1] = temp;
            }
            if ((all_characters_community_two[j]->name).compare(all_characters_community_two[j + 1]->name) > 0)
            {
                Character* temp = all_characters_community_two[j];
                all_characters_community_two[j] = all_characters_community_two[j + 1];
                all_characters_community_two[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    string infile_name = argv[1];
    string outfile_name = argv[2];

    ifstream inputFile;
    inputFile.open(infile_name);

    ofstream outputFile;
    outputFile.open(outfile_name);

    Character** all_characters = new Character * [10]; // all characters.
    Character** all_characters_community_one = new Character * [5]; // all characters of community one.
    Character** all_characters_community_two = new Character * [5]; // all characters of community two.

    bool draw = false; 
    bool result = false;
    int count = 1; // this variable implies which raund are we in. if count %2 == 0 then com1 attacks to com2, else com2 attacks to com2. 
    string line;

    int maxRaunds;
    string name;
    string type;
    int attack_point;
    int defense_point;
    int initial_health;

    string attacker;
    string defender;

    int last_killed_one = -1; // last killed member of community one.
    int last_killed_two = -1; // last killed member of community two.


    if (inputFile.is_open())
    {

        while (getline(inputFile, line))
        {

            istringstream iss(line);
            if (count == 1) // reading max raunds.
            {
                iss >> maxRaunds;
            }
            if (count > 1 && count < 7) // Creating characters of community 1
            {

                iss >> name >> type >> attack_point >> defense_point >> initial_health;
                Character* character = new Character(name, type, attack_point, defense_point, initial_health, maxRaunds);
                all_characters_community_one[count - 2] = character;
                all_characters[count - 2] = character;

            }

            if (7 <= count && count < 12) // Creating characters of community 2
            {
                iss >> name >> type >> attack_point >> defense_point >> initial_health;
                Character* character = new Character(name, type, attack_point, defense_point, initial_health, maxRaunds);
                all_characters_community_two[count - 7] = character;
                all_characters[count - 2] = character;

            }

            if (count == 12) // sorting in terms of alphabetic order.
            {
                alphabetic_order(all_characters_community_one, all_characters_community_two);
            }

            if (count >= 12) // war simulation.
            {
                int i = 0;
                int k = 0;
                bool flag = false;
                int w;
                if (count % 2 == 0) // community 1 attacks.
                {
                    iss >> name; // attacker
                    for (i = 0; i < 5; i++) // searching specified attacker character.
                    {
                        if (all_characters_community_one[i]->name.compare(name) == 0)
                        {
                            if (all_characters_community_one[i]->isAlive)
                            {
                                break;
                            }
                            else
                            { // if specified character is dead, then find alphabetical next character, if there is no alphabetical next character then find alphabetical previous character.
                                w = i + 1;
                                while (w < 5)
                                {
                                    if (all_characters_community_one[w]->isAlive)
                                    {
                                        flag = true;
                                        i = w;
                                        break;
                                    }
                                    w++;
                                }
                                w = i - 1;
                                while (w > -1 && !flag)
                                {
                                    if (all_characters_community_one[w]->isAlive)
                                    {
                                        i = w;
                                        break;
                                    }
                                    w--;
                                }
                                break;
                            }

                        }
                    }
                    iss >> name; // defender
                    for (k = 0; k < 5; k++) // searching specified defender character.
                    {
                        if (all_characters_community_two[k]->name == name)
                        {
                            if (all_characters_community_two[k]->isAlive)
                            {
                                break;
                            }
                            else // if specified character is dead, then find alphabetical next character, if there is no alphabetical next character then find alphabetical previous character.
                            { 
                                w = k + 1;
                                while (w < 5)
                                {
                                    if (all_characters_community_two[w]->isAlive)
                                    {
                                        flag = true;
                                        k = w;
                                        break;
                                    }
                                    w++;
                                }
                                w = k - 1;
                                while (w > -1 && !flag)
                                {
                                    if (all_characters_community_two[w]->isAlive)
                                    {
                                        k = w;
                                        break;
                                    }
                                    w--;
                                }
                                break;
                            }

                        }
                    }
                    iss >> name; // special attack or not
                    if (name == "NO-SPECIAL") // no special attack.
                    {
                        attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);
                        update_communities(all_characters_community_one, all_characters_community_two, count);
                    }
                    else if (name == "SPECIAL") // special attack.
                    {
                        if (all_characters_community_one[i]->type == "Elves") // if character is elves.
                        {
                            if (all_characters_community_one[i]->nRoundsSinceSpecial >= 10) // elves can use their special skill after nRoundsSinceSpecial field >10
                            {
                                int m;
                                for (m = 0; m < 5; m++)
                                {
                                    if (all_characters_community_one[m]->type == "Hobbit")  // searching hobbit character.
                                    {
                                        break;
                                    }
                                }
                                // elves convey half of their health to hobbits.
                                all_characters_community_one[m]->remainingHealth = all_characters_community_one[m]->remainingHealth + ((all_characters_community_one[i]->remainingHealth) / 2);
                                all_characters_community_one[i]->remainingHealth = all_characters_community_one[i]->remainingHealth - all_characters_community_one[i]->remainingHealth / 2;
                                all_characters_community_one[i]->nRoundsSinceSpecial == -1;
                            }
                            // attacking and updating raund.
                            attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);
                            update_communities(all_characters_community_one, all_characters_community_two, count);
                        }
                        else if (all_characters_community_one[i]->type == "Dwarfs") // if character is dwarf.
                        {
                            if (all_characters_community_one[i]->nRoundsSinceSpecial >= 20) // dwarfs can use their special skill after nRoundsSinceSpecial field >20
                            {
                                // attacks two times to the opponent.
                                attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);
                                attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);

                                all_characters_community_one[i]->nRoundsSinceSpecial == -1;
                                update_communities(all_characters_community_one, all_characters_community_two, count);
                            }
                            else // if nRoundsSinceSpecial < 20
                            {
                                attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);
                                update_communities(all_characters_community_one, all_characters_community_two, count);
                            }
                        }
                        else if (all_characters_community_one[i]->type == "Wizards") // if character is wizard.
                        {

                            if (all_characters_community_one[i]->nRoundsSinceSpecial >= 50) // dwarfs can use their special skill after nRoundsSinceSpecial field >20
                            {

                                if (last_killed_one != -1) // if there is dead character in community one.
                                {
                                    if (all_characters_community_one[last_killed_one]->isAlive == true) // if that character is alive then just refill his health and do not reset his nRoundsSinceSpecial field.
                                    {
                                        all_characters_community_one[last_killed_one]->remainingHealth = all_characters_community_one[last_killed_one]->healthHistory[0];
                                    }
                                    else if (all_characters_community_one[last_killed_one]->isAlive == false) // if that character is dead then resurrect him and set his nRoundsSinceSpecial field to -1.
                                    {
                                        all_characters_community_one[last_killed_one]->nRoundsSinceSpecial = -1; 
                                        all_characters_community_one[last_killed_one]->isAlive = true;
                                        all_characters_community_one[last_killed_one]->remainingHealth = all_characters_community_one[last_killed_one]->healthHistory[0];
                                    }
                                    all_characters_community_one[i]->nRoundsSinceSpecial = -1; // set witch's nRoundsSinceSpecial field to -1.

                                }

                            }

                            attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);
                            update_communities(all_characters_community_one, all_characters_community_two, count);

                        }
                        else if (all_characters_community_one[i]->type == "Men" || all_characters_community_one[i]->type == "Hobbit") // if that character is men or hobbit just attack like no-special case.
                        {
                            attack(*(all_characters_community_two[k]), *(all_characters_community_one[i]), last_killed_one, last_killed_two, k, i, count);
                            update_communities(all_characters_community_one, all_characters_community_two, count);
                        }
                        else // for invalid inputs.
                        {
                            update_communities(all_characters_community_one, all_characters_community_two, count);
                        }
                    }
                }


                else // This else block is just like above if block. Only difference is now community2 characters attacks to community1 characters.
                {
                    iss >> name; // attacker
                    for (i = 0; i < 5; i++) 
                    {
                        if (all_characters_community_two[i]->name == name) 
                        {
                            if (all_characters_community_two[i]->isAlive)
                            {

                                break;
                            }
                            else
                            {

                                w = i + 1;
                                while (w < 5)
                                {
                                    if (all_characters_community_two[w]->isAlive)
                                    {
                                        flag = true;
                                        i = w;
                                        break;
                                    }
                                    w++;
                                }
                                w = i - 1;
                                while (w > -1 && !flag)
                                {
                                    if (all_characters_community_two[w]->isAlive)
                                    {
                                        i = w;
                                        break;
                                    }
                                    w--;
                                }


                                break;
                            }
                        }
                    }

                    iss >> name; // defender
                    for (k = 0; k < 5; k++) 
                    {
                        if (all_characters_community_one[k]->name == name)
                        {
                            if (all_characters_community_one[k]->isAlive)
                            {

                                break;
                            }
                            else
                            {

                                w = k + 1;
                                while (w < 5)
                                {
                                    if (all_characters_community_one[w]->isAlive)
                                    {
                                        flag = true;
                                        k = w;
                                        break;
                                    }
                                    w++;
                                }
                                w = k - 1;
                                while (w > -1 && !flag)
                                {
                                    if (all_characters_community_one[w]->isAlive)
                                    {
                                        k = w;
                                        break;
                                    }
                                    w--;
                                }
                                break;
                            }
                        }
                    }
                    iss >> name;
                    if (name == "NO-SPECIAL") 
                    {
                        attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);
                        update_communities(all_characters_community_two, all_characters_community_one, count);
                    }
                    else if (name == "SPECIAL") 
                    {
                        if (all_characters_community_two[i]->type == "Elves")
                        {
                            if (all_characters_community_two[i]->nRoundsSinceSpecial >= 10)
                            {
                                int m;
                                for (m = 0; m < 5; m++)
                                {
                                    if (all_characters_community_two[m]->type == "Hobbit")
                                    {
                                        break;
                                    }
                                }
                                all_characters_community_two[m]->remainingHealth += all_characters_community_two[i]->remainingHealth / 2;
                                all_characters_community_two[i]->remainingHealth -= all_characters_community_two[i]->remainingHealth / 2;
                                all_characters_community_two[i]->nRoundsSinceSpecial == -1;
                            }
                            attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);
                            update_communities(all_characters_community_two, all_characters_community_one, count);
                        }
                        else if (all_characters_community_two[i]->type == "Dwarfs")
                        {
                            if (all_characters_community_two[i]->nRoundsSinceSpecial >= 20) 
                            {
                                attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);
                                attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);

                                all_characters_community_two[i]->nRoundsSinceSpecial == -1;
                                update_communities(all_characters_community_two, all_characters_community_one, count);
                            }
                            else
                            {
                                attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);
                                update_communities(all_characters_community_two, all_characters_community_one, count);
                            }
                        }
                        else if (all_characters_community_two[i]->type == "Wizards") 
                        {

                            if (all_characters_community_two[i]->nRoundsSinceSpecial >= 50)
                            {
                                if (last_killed_two != -1)
                                {                                   
                                    if (all_characters_community_two[last_killed_one]->isAlive != false)
                                    {
                                        all_characters_community_two[last_killed_one]->remainingHealth = all_characters_community_two[last_killed_one]->healthHistory[0];
                                    }
                                    else if (all_characters_community_two[last_killed_one]->isAlive == false)
                                    {
                                        all_characters_community_two[last_killed_one]->nRoundsSinceSpecial = -1; 
                                        all_characters_community_two[last_killed_one]->isAlive = true;
                                        all_characters_community_two[last_killed_one]->remainingHealth = all_characters_community_two[last_killed_one]->healthHistory[0];
                                    }
                                    all_characters_community_two[i]->nRoundsSinceSpecial = -1;

                                }

                            }

                            attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);
                            update_communities(all_characters_community_two, all_characters_community_one, count);

                        }
                        else if (all_characters_community_two[i]->type == "Men" || all_characters_community_two[i]->type == "Hobbit")
                        {
                            attack(*(all_characters_community_one[k]), *(all_characters_community_two[i]), last_killed_one, last_killed_two, k, i, count);
                            update_communities(all_characters_community_two, all_characters_community_one, count);
                        }
                        else
                        {
                            update_communities(all_characters_community_two, all_characters_community_one, count);
                        }
                    }

                }
            }

            if (count > 12) // This section checks that whether war is over or not.
            {              
                if (count - 11 == all_characters_community_one[0]->nMaxRounds) // if we reach to maxraunds then result is draw.
                {
                    draw = true;
                }
                result = is_it_over(all_characters_community_one, all_characters_community_two, count, draw); // if war is over then return true.
                if (result) // if war is over then write war results in detail.
                {
                    int deaths = 0;
                    if (draw)
                    {
                        outputFile << "Draw" << endl;
                    }
                    else if (count % 2 == 0)
                    {
                        outputFile << "Community-1" << endl;
                    }
                    else
                    {
                        outputFile << "Community-2" << endl;
                    }
                    outputFile << count - 11 << endl;

                    for (int j = 0; j < 5; j++)
                    {
                        if (!all_characters_community_one[j]->isAlive)
                        {
                            deaths++;
                        }
                        if (!all_characters_community_two[j]->isAlive)
                        {
                            deaths++;
                        }
                    }
                    outputFile << deaths;

                    for (int i = 0; i < 10; i++)
                    {

                        outputFile << endl;
                        outputFile << all_characters[i]->name << " ";
                        for (int j = 0; j < count - 10; j++)
                        {
                            outputFile << all_characters[i]->healthHistory[j] << " ";
                        };
                    }

                }
            }
            if (result)
            {
                break;
            }


            count++;
        }

        inputFile.close();
    }
    //deleting arrays...
    delete[] all_characters;
    for(int i = 0 ; i<5 ; i++)
    {
        delete all_characters_community_one[i];
        delete all_characters_community_two[i];
    }
    delete[] all_characters_community_one;
    delete[] all_characters_community_two;
    outputFile.close();
    return 0;
}
