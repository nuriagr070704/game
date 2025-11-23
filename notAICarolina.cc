#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Aguacate


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  const vector<Dir> direct = {Up,Down,Right,Left}; //direcciones
  typedef vector<int> Row;
  typedef vector<Row> Matrix;
  // typedef pair<Pos,Pos> Movi;
  // typedef pair<int,Movi> Move;
  // typedef pair<int,Move> Movement;

  bool cell_valid(const Pos& next, const Matrix& dist_vis) {
    if (pos_ok(next) and dist_vis[next.i][next.j] == -1) {
      Cell c_next = cell(next);
      return c_next.type == Street and c_next.id == -1;
    }
    return false;
  }

  bool cell_valid_with_citizens(const Pos& next, const Matrix& dist_vis) {
    if (pos_ok(next) and dist_vis[next.i][next.j] == -1) {
      Cell c_next = cell(next);
      return c_next.type == Street;
    }
    return false;
  }

  bool weak_bazooka(const Citizen& c) {
    return c.player != me() and (c.type == Builder or (c.type == Warrior and c.weapon < Bazooka));
  }

  bool weak_gun(const Citizen& c) {
    return c.player != me() and (c.type == Builder or (c.type == Warrior and c.weapon <= Gun));
  }

  bool weak_hammer(const Citizen& c) {
    return c.player != me() and (c.type == Builder or c.weapon == Hammer);
  }

  Pos BFS_weak_bazooka(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid_with_citizens(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;

          if (c_next.id != -1) {
            Citizen c = citizen(c_next.id);
            if (weak_bazooka(c)) return next_step1;
          }
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; 
  }

  Pos BFS_weak_gun(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid_with_citizens(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;

          if (c_next.id != -1) {
            Citizen c = citizen(c_next.id);
            if (weak_gun(c) and dist_vis[next.i][next.j] <= 2) return next_step1;
          }
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; 
  }

  Pos BFS_weak_hammer(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid_with_citizens(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;

          if (c_next.id != -1) {
            Citizen c = citizen(c_next.id);
            if (weak_hammer(c) and dist_vis[next.i][next.j] <= 2) return next_step1;
          }
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; 
  }

  Pos BFS_bonus(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;
          
          if (c_next.id != 1 and (c_next.bonus == Food or c_next.bonus == Money)) return next_step1;
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; //si no encuentra FOod o Money, que retorne posición inicial
  }

  Pos BFS_bazooka(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;

          if (c_next.weapon == Bazooka) return next_step1;
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; //si no encuentra Bazooka, que retorne posición inicial
  }

  Pos BFS_take_all_weapons(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;

          if (c_next.weapon == Gun or c_next.weapon == Hammer) return next_step1;
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; //si no hay armas retorna ini
  }

  Pos BFS_near_enemy(Pos& p, int n, int m) {
    queue<pair<Pos,Pos>> Q;
    Matrix dist_vis(n,Row(m,-1));
    Pos ini = p;
    Pos visi; //posicion visitada
    Pos next; //posicion siguiente
    Pos step1; //paso preliminar antes de ir al definitivo
    Pos next_step1; //para conservar el paso preliminar correctamente

    Q.push({ini,ini});
    dist_vis[ini.i][ini.j] += 1; //ahora es 0

    while (not Q.empty()) {
      visi = Q.front().first;
      step1 = Q.front().second; 
      Q.pop();
      
      for (uint u = 0; u < direct.size(); ++u) {
        next = visi + direct[u];

        if (cell_valid_with_citizens(next,dist_vis)) {
          Cell c_next = cell(next);
          if (ini == step1) next_step1 = next;
          else next_step1 = step1;

          if (c_next.id != -1) {
            Citizen c = citizen(c_next.id);
            if (c.player != me() and c.type == Warrior) return next_step1;
          }
          Q.push({next,next_step1});
          dist_vis[next.i][next.j] = dist_vis[visi.i][visi.j] + 1;
        }
      }
    }
    return ini; 
  }
  
  void movements(const Pos& ini, const Pos& baz, int id_citizen) {
    if (ini.i < baz.i) move(id_citizen,Down);
    else if (ini.i > baz.i) move(id_citizen,Up);
    else if (ini.j < baz.j) move(id_citizen,Right);
    else if (ini.j > baz.j) move(id_citizen,Left);
  }

  void invers_movements(const Pos& ini, const Pos& baz, int id_citizen) {
    if (ini.i < baz.i) move(id_citizen,Up);
    else if (ini.i > baz.i) move(id_citizen,Down);
    else if (ini.j < baz.j) move(id_citizen,Left);
    else if (ini.j > baz.j) move(id_citizen,Right);
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    double st = status(me());
    if (st >= 0.9) return;

    int n = board_rows();
    int m = board_cols();

    //priority_queue<Movement> PQ;

    vector<int> W = warriors(me()); //todos los guerreros de me()
    for (uint v = 0; v < W.size(); ++v) { //miro todos sus guerreros
      Citizen guer = citizen(W[v]);
      Pos ini_w; //inicializo su posicion

      if (guer.weapon == Bazooka) {
        ini_w = guer.pos;
        Pos riv = BFS_weak_bazooka(ini_w,n,m); //si tengo bazooka mato a rivales
        if (riv != ini_w) movements(ini_w,riv,guer.id); //PQ.push({2,{guer.id,{ini_w,riv}}});
        else { 
          ini_w = guer.pos;
          riv = BFS_take_all_weapons(ini_w,n,m); //si no hay rivales pillo cualquier otra arma
          movements(ini_w,riv,guer.id); //PQ.push({4,{guer.id,{ini_w,riv}}});
        }
      }
      else if (guer.weapon == Gun) {
        ini_w = guer.pos;
        Pos kill = BFS_weak_gun(ini_w,n,m); //si tengo pistola mato a rivales
        if (kill != ini_w) movements(ini_w,kill,guer.id); //PQ.push({3,{guer.id,{ini_w,kill}}});
        else {
          ini_w = guer.pos;
          kill = BFS_take_all_weapons(ini_w,n,m); //si no encuentro rivales pillo cualquier otra arma
          movements(ini_w,kill,guer.id); //PQ.push({4,{guer.id,{ini_w,kill}}});
        }
      }
      else {
        ini_w = guer.pos;
        Pos baz = BFS_bazooka(ini_w,n,m); //busco bazooka
        if (baz != ini_w) movements(ini_w,baz,guer.id); //PQ.push({1,{guer.id,{ini_w,baz}}});
        else {
          ini_w = guer.pos;
          baz = BFS_weak_hammer(ini_w,n,m); //si no encuentro bazooka mato a rivales
          if (baz != ini_w) movements(ini_w,baz,guer.id); //PQ.push({5,{guer.id,{ini_w,baz}}});
          else {
            baz = BFS_take_all_weapons(ini_w,n,m); //si no encuentro rivales pillo cualquier otra arma
            movements(ini_w,baz,guer.id); //PQ.push({4,{guer.id,{ini_w,baz}}});
          }      
        }  
      }
    }

    vector<int> B = builders(me()); //todos los constructores de me()
    for (uint x = 0; x < B.size(); ++x) { //miro todos sus constructores
      Citizen bldr = citizen(B[x]);
      Pos ini_b = bldr.pos; //inicializo su posicion
      Pos bon = BFS_bonus(ini_b,n,m); //busco comida o monedas
      if (bon != ini_b) movements(ini_b,bon,bldr.id); //PQ.push({6,{bldr.id,{ini_b,bon}}});
      else {
        ini_b = bldr.pos;
        bon = BFS_near_enemy(ini_b,n,m);
        if (ini_b != bon) invers_movements(ini_b,bon,bldr.id);
        else {
          ini_b = bldr.pos;
          bon = BFS_take_all_weapons(ini_b,n,m); //si no encuentro pillo cualquier arma
          movements(ini_b,bon,bldr.id); //PQ.push({7,{bldr.id,{ini_b,bon}}});
        } 
      } 
    }
    // while (not PQ.empty()) {
    //   int id = PQ.top().second.first;
    //   Pos inicial = PQ.top().second.second.first;
    //   Pos a_moure = PQ.top().second.second.second;
    //   PQ.pop();
    //   movements(inicial,a_moure,id);
    // }
  }
};

        

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);