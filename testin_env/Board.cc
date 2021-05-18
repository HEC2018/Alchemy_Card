
#include "Board.h"
#include <bits/stdc++.h>
using namespace std;


Board::Board(string p1name, string p2name,string deck1,string deck2){
    Player* p1 = new Player(p1name,deck1);
    Player* p2 = new Player(p2name,deck2);
    
    players.push_back(p1);
    players.push_back(p2);

    vector<Minion*> g1,g2,m1,m2;
    vector<Ritual*> r1,r2;
    r1.push_back(nullptr);
    r2.push_back(nullptr);


    graveyard.push_back(g1);
    graveyard.push_back(g2);
    minions.emplace_back(m1);
    minions.emplace_back(m2);
    rituals.push_back(r1);
    rituals.push_back(r2);

    td = new TextDisplay();
    //players[player_idx]->shuffle();
    //players[player_idx^1]->shuffle();

}

Board::~Board(){
    delete td;
    //GraphicDisplay *gd = nullptr;
    for (int i = 0 ; i < players.size();i++){
        delete players[i];
    }

    for (int i = 0 ; i < graveyard.size();i++){
        for (int j = 0 ; j < graveyard[i].size(); j++){
            delete graveyard[i][j];
        }
    }

    for (int i = 0 ; i < minions.size();i++){
        for (int j = 0 ; j < minions[i].size();j++){
            delete minions[i][j];
        }
    }

    for (int i = 0 ; i < rituals.size();i++){
        for (int j = 0 ; j < rituals[i].size();j++){
            delete rituals[i][j];
        }
    }
}

void Board::shuffle(int testing){
    if(testing){
        players[player_idx]->shuffle();
        players[player_idx^1]->shuffle();
    }
    for(int i =0; i < 5; ++i){
        players[player_idx]->draw();
        players[player_idx^1]->draw();
    }
}

void Board::drawCard(){
    players[player_idx]->draw();
}

void Board::drawMinion(int idx){
    cout << (td->inspectMinion(minions[player_idx][idx])) << endl;
}
void Board::discardCard(int idx){
    players[player_idx]->discard(idx);
}
void Board::drawBoard(){
    cout << (td->drawBoard(players,minions,rituals,graveyard)) << endl;
}

void Board::drawHand(){
    cout << (td->drawHand(players[player_idx]->getHand()));
}
string Board::getPlayerName(int idx){
    return players[idx]->getName();
}
int Board::getWinner() const{
    return winner;
}
void Board::help() {
    cout<<"Commands: help -- Display this message.\n"
        "          end  -- End the current player's turn.\n"
        "          quit -- End the game.\n"
        "          attack minion other-minion -- Orders minion to attack other-minion.\n"
        "          attack minion -- Orders minion to attack the opponent.\n"
        "          play card [target-player target-card] -- Play card, optionally targeting target-card owned by target-player.\n"
        "          use minion [target-player target-card] -- Use minion's special ability, optionally targeting target-card owned by target-player.\n"
        "          inspect minion -- View a minion's card and all enchantments on that minion.\n"
        "          hand -- Describe all cards in your hand.\n"
        "          board -- Describe all cards on the board.\n";
}



void Board::applyAll(Card* c){
    players[player_idx]->apply(c); // Change the player.apply(c)
    for(int i = 0; i < minions[player_idx].size(); ++i){
        minions[player_idx][i]->apply(c);
        die(minions[player_idx][i]);
    }

    if (rituals[player_idx][0]){
        rituals[player_idx][0]->apply(c);
    }

    players[player_idx^1]->apply(c); // Change the player.apply(c)
    for(int i = 0; i < minions[player_idx^1].size(); ++i){
        minions[player_idx^1][i]->apply(c);
        die(minions[player_idx^1][i]);
    }

    if (rituals[player_idx^1][0]){
        rituals[player_idx^1][0]->apply(c);
    } 

    return;
}

void Board::applySingle(Card* c){
    const Effect* const e = c->getEffect();
    const Player* const p = c->getPlayer();

    if( (e->player == true) && (e->zone == "Allied") && (p == players[player_idx])){        
        players[player_idx]->changeMagic(1);       
        return;
    }else if(e->self){
        c->apply(c);
        die(dynamic_cast<Minion*>(c));
        return;
    }else if(((e->zone == "Allied") || (e->zone == "All")) && (p == players[player_idx])){
        cerr << "Someone applied " << c->getName() << endl;
        minions[player_idx].back()->apply(c);

        die(minions[player_idx].back());
        cerr << "hi" << endl;
        return;
    }else if((e->zone == "Opponent" || e->zone == "All") && p != players[player_idx]){
        //cerr << "Someone applied " << (minions.at(player_idx^1).back() == nullptr) << endl;
        minions[player_idx].back()->apply(c);

        die(minions[player_idx].back());
        return;
    }

}

void Board::die(const Minion* const c){
    if(c->isDead()){
        for(int i = 0; i < minions[player_idx].size(); ++i){
            if(minions[player_idx][i] == c){
                graveyard[player_idx].emplace_back(minions[player_idx][i]);

                minions[player_idx].erase(minions[player_idx].begin() + i);
                notifyAll(Trigger::MLeave);
                cerr << "Someone dies: Minions"<<player_idx+1<<" has " << minions[player_idx].size() << endl;
                return;
            }
        }

        for(int i = 0; i < minions[player_idx^1].size(); ++i){
            if(minions[player_idx^1][i] == c){
                graveyard[player_idx^1].emplace_back(minions[player_idx^1][i]);
                minions[player_idx^1].erase(minions[player_idx^1].begin() + i);
                notifyAll(Trigger::MLeave);
                cerr << "Someone dies: Minions"<< (player_idx^1)+1 << " has " << minions[player_idx^1].size() << endl;
                return;
            }
        }
    }
    return ;

}

void Board::notifyAll(Trigger t){
    using namespace std;
    cerr << "player notify begin" << endl;
    players[player_idx]->notify(t);
    cerr << "player notify finished" << endl;

    cerr << "minions notify begin" << endl;
    for(int i = 0; i < minions[player_idx].size(); ++i){

        Minion* c = minions[player_idx][i]->notify(t);

        if(c){
            if(c->getEffect()->single){
                applySingle(c);
            }else{
                applyAll(c);
            }
        }
        cerr << "minions " << player_idx << " notify end" << endl;
    }
    cerr << "minions notify finished" << endl;
    cerr << "ritual notify begin" << endl;
    if(rituals[player_idx][0]){
        Ritual *c = rituals[player_idx][0]-> notify(t);

        if(c){
            if(c->getEffect()->single){
                applySingle(c);
            }else{
                applyAll(c);
            }
        }
    }
    cerr << "ritual notify finished" << endl;


    if(t != Trigger::Start && t!= Trigger::End){
        cerr << "notify opponent begin" << endl;
        for(int i = 0; i < minions[player_idx^1].size(); ++i){
            Card *c = minions[player_idx^1][i]->notify(t);
            if(c){
                if(c->getEffect()->single){
                    applySingle(c);
                }else{
                    applyAll(c);
                }
            }
        }

        cerr << "notify ritual opponenet begin" << endl;
        if(rituals[player_idx^1][0]){
            Card *c = rituals[player_idx^1][0] -> notify(t);
            if(c){
                if(c->getEffect()->single){
                    cout << "single" << endl;
                    applySingle(c);
                }else{
                    applyAll(c);
                }
            }

        }
    }
    cerr << "ritual notify opponent end" << endl;
}

void Board::startTurn(){
    notifyAll(Trigger::Start);
    players[player_idx]->draw();
}

void Board::endTurn(){
    notifyAll(Trigger::End);
    player_idx^=1;
}

void Board::attack(int i, int j){
    cerr << "Attack begin" << endl;

    if(minions[player_idx][0]->getAct()){
        int atk1 = minions[player_idx^1][j]->getAtk();
        //Minion* m = new Minion(*(minions[player_idx^1][j]));
        minions[player_idx^1][j]->collision(minions[player_idx][i]);
        minions[player_idx][0]->action();

        cerr << "Attacking" << endl;
        minions[player_idx][i]->collision(minions[player_idx^1][j]);
        die(minions[player_idx][i]);
        die(minions[player_idx^1][j]);

    }

    cerr << "Attack End" << endl;
}

void Board::attack(int i){
    if(minions[player_idx][0]->getAct()){
        players[player_idx^1]->collision(minions[player_idx][i]);
        minions[player_idx][0]->action();
    }
    if(players[player_idx^1]->isDead()){
        winner = player_idx^1;
    }
}

void Board::play(int idx, int testing){
    Card *c = players[player_idx]->getCard(idx, testing);
    if(c == nullptr) {
        cerr << "Not enough Magic"<<endl;
        return;
    }
    const string ty = c->getType(); 
    if(ty == "Enchantment"){
        cerr << "Wrong Command" << endl;
        return;
    }
    if(ty == "Minion"){
        if(minions[player_idx].size() < 5){
            cerr << "Summon Begin" << endl;
            minions[player_idx].emplace_back(dynamic_cast<Minion*>(c));
            players[player_idx]->summon(idx);
            cerr << "Summon End" << endl;
            cerr << "notify Begin" << endl;
            notifyAll(Trigger::MEntry);
            cerr << "notify Finish" << endl;
            return;
        }
        cerr << "Wrong Command" << endl;
        return;
    }
    if(ty == "Spell"){
        if(c->getEffect()->single){
            cerr << "Wrong Command" << endl;
            return;
        } else if(c->getEffect()->resurrect){
            if (!graveyard[player_idx].empty() && minions[player_idx].size() < 5){\
                graveyard[player_idx].back()->resurrect();
                minions[player_idx].push_back(graveyard[player_idx].back());
                graveyard[player_idx].pop_back();
            }

        }else {
            players[player_idx]->summon(idx);
            applyAll(c);
            return;
        }
    }
    if(ty == "Ritual"){
        players[player_idx]->summon(idx);
        delete rituals[player_idx][0];
        rituals[player_idx].clear();
        rituals[player_idx].emplace_back(dynamic_cast<Ritual*>(c));
        return;
    }
}


void Board::play(int idx, int j, int a_idx, int testing){
    Card *c = (Card*)players[player_idx]->getCard(idx, testing);
    if(c == nullptr) {
        cerr << "Not enough Magic"<<endl;
        return;
    }
    const string ty = c->getType(); 
    if(ty == "Enchantment"){
        if(a_idx == 5){
            cerr << "Wrong Command, cannot be Enchantment" << endl;
            return;
        } else {
            minions[j][a_idx]->apply(c);
            players[player_idx]->summon(idx);
            return;
        }}
    if(ty == "Minion"){
        cerr << "Wrong Command, cannot be minion" << endl;
        return;
    }
    if(ty == "Spell"){
        if( c->getEffect()->single == 0){
            cerr << "Wrong Command, the spell shal single" << !(c->getEffect()->single) << endl;
            return;
        } else if (c->getEffect()->unsummon){
            try{
                players[j]->unsummon(minions[j][a_idx]);
                minions[j].erase(minions[j].begin()+a_idx);
                players[player_idx]->summon(idx);
            }catch(string e){
                cerr << "Error" << endl;
            }
        }else{
            if(a_idx == 5){
                if(rituals[j][0]){
                    if(c->getEffect()->kill){
                        delete rituals[player_idx][0];
                        rituals[player_idx].clear();
                        players[player_idx]->summon(idx);
                        return;
                    }
                    rituals[j][0]->apply(c);
                    players[player_idx]->summon(idx);
                    return;
                } else {
                    cerr << "Wrong Command, the ritual does not exist" <<endl;
                    return;
                } 
            }else {
                minions[j][a_idx]->apply(c);
                die(minions[j][a_idx]);
            }
            return;
        }
    }
    if(ty == "Ritual"){
        cerr << "Wrong Command, shall not be ritual" << endl;
        return;
    }
}


void Board::use(int idx, int testing){
    Minion *m = minions[player_idx][idx];
    if(m->getAct()){
        if( (m->getActCost() <= players[player_idx]->getMagic() || testing ) && m->getActCost() != -1){
            const Effect * const ef = m->getActiveSkill(); 
            if(ef && !(ef->single)) {
                int n = m->getInfo()->amount;
                cout << n << endl;
                for(int i = 0; i < n; ++i){

                    if(minions[player_idx].size() < 5){
                        Minion *m1 = new Minion(players[player_idx], "Air Elemental", "", 0, 1,1, Trigger::NoTrigger);
                        minions[player_idx].emplace_back(m1);
                        notifyAll(Trigger::MEntry);
                    }
                }
                m->action();
                if(!testing){
                    players[player_idx]->changeMagic(-1*m->getActCost());
                }
                return;
            }

        }

    }
    cerr << "Wrong command, no action avaliable "<<endl;
}


void Board::use(int idx, int j, int a_idx, int testing){
    Minion *m = minions[player_idx][idx];
    cerr << "HI" << m->getActiveSkill()->unsummon << endl;
    if(m->getAct()){
        if( (m->getActCost() <= players[player_idx]->getMagic() || testing )&& m->getActCost() != -1){
            if(m->getActiveSkill()->single)
            {
                minions[j][a_idx]->apply(dynamic_cast<Card*>(m));
                die(minions[j][a_idx]);
                m->action();
                if(!testing){
                    players[player_idx]->changeMagic(-1*m->getActCost());
                }
                return;
            }

        }

    }

}


