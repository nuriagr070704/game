#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Shimbaeh


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

  //Direccion para ir de `a` a `b`
  Dir go_to(Pos a, Pos b) {
    vector<Dir> dirs;

    //mira si es mejor ir arriba o abajo
    if (b.i > a.i && pos_ok(a+Down) && cell(a+Down).type != Wall) dirs.push_back(Down);
    else if (b.i < a.i && pos_ok(a+Up) && cell(a+Up).type != Wall) dirs.push_back(Up);

    //mira si es mejor ir derecha o izquierda
    if (b.j > a.j && pos_ok(a+Right) && cell(a+Right).type != Wall) dirs.push_back(Right);//derecha si el destino 
    else if (b.j < a.j && pos_ok(a+Left) && cell(a+Left).type != Wall) dirs.push_back(Left);

    //devuelve la primera opcion disponible
    if (dirs.size() != 0) return dirs[0];
    else {
      //si no habia, devuelve una disponible
      for (Dir d : {Down, Right, Up, Left}) {
        Pos next = a + d;
        if (pos_ok(next) && cell(next).type != Wall) return d;
      }
    }
    return Down;
  }

  const vector<int> dir_x = {1, 0, -1, 0};
  const vector<int> dir_y = {0, 1, 0, -1};

  //posicion a seguir para llegar a soldados o profesores enemigos
  Pos BFS_kill_soldiers_or_professors(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<Pos,Pos>> q;
    Pos next_step_pos;

    q.push({p, p});
    dist[p.i][p.j] = 0;

    while (!q.empty()) {
      int x = q.front().first.i;
      int y = q.front().first.j;
      Pos step_pos = q.front().second;
      q.pop();

      for (int i = 0; i<4; i++) {
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];

        if (pos_ok(nx, ny)) {
          Cell nc = cell(nx,ny);
          if (nc.type != Wall && dist[nx][ny] == -1) {
            if (p == step_pos) next_step_pos = Pos(nx, ny);
            else next_step_pos = step_pos;

            if (nc.id != -1) {
              Unit u = unit(nc.id);
              if (u.player != me()) return next_step_pos;
            }

            dist[nx][ny] = dist[x][y] + 1;
            q.push({Pos(nx,ny), next_step_pos});
          }
        }
      }
    }
    return p;
  }

  Pos BFS_take_poison_money(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<Pos,Pos>> q;
    Pos next_step_pos;

    q.push({p, p});//{"posicion a mirar", "paso a seguir desde la posicion inicial"}
    dist[p.i][p.j] = 0;

    while (!q.empty()) {
      int x = q.front().first.i;
      int y = q.front().first.j;
      Pos step_pos = q.front().second;
      q.pop();

      for (int i = 0; i<4; i++) {//mirar todas las direcciones
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];

        if (pos_ok(nx, ny)) {
          Cell nc = cell(nx,ny);
          if (nc.type != Wall && dist[nx][ny] == -1) {//comprovar que no es una pos invalida
            if (p == step_pos) next_step_pos = Pos(nx, ny);//propagar el paso a seguir para llegar al objetivo
            else next_step_pos = step_pos;

            if (nc.poison_kit || nc.money != 0) return next_step_pos;//si se ha llegado al objetivo, devolver la pos a seguir

            dist[nx][ny] = dist[x][y] + 1;//aumentar la distancia
            q.push({Pos(nx,ny), next_step_pos});//añadir la nueva posicion visitada a la cola con el paso incial a seguir
          }
        }
      }
    }
    return p;//devolver la inicial si no encuentra el objetivo, imposible que pase
  }

  //posicion a seguir para llegar a kits o dinero
  Pos BFS_take_kits_or_money(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<Pos,Pos>> q;
    Pos next_step_pos;

    q.push({p, p});//{"posicion a mirar", "paso a seguir desde la posicion inicial"}
    dist[p.i][p.j] = 0;

    while (!q.empty()) {
      int x = q.front().first.i;
      int y = q.front().first.j;
      Pos step_pos = q.front().second;
      q.pop();

      for (int i = 0; i<4; i++) {//mirar todas las direcciones
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];

        if (pos_ok(nx, ny)) {
          Cell nc = cell(nx,ny);
          if (nc.type != Wall && dist[nx][ny] == -1) {//comprovar que no es una pos invalida
            if (p == step_pos) next_step_pos = Pos(nx, ny);//propagar el paso a seguir para llegar al objetivo
            else next_step_pos = step_pos;

            if (nc.health_kit || nc.poison_kit || nc.money>0) return next_step_pos;//si se ha llegado al objetivo, devolver la pos a seguir

            dist[nx][ny] = dist[x][y] + 1;//aumentar la distancia
            q.push({Pos(nx,ny), next_step_pos});//añadir la nueva posicion visitada a la cola con el paso incial a seguir
          }
        }
      }
    }
    return p;//devolver la inicial si no encuentra el objetivo, imposible que pase
  }

  //posicion a seguir para llegar a una caja
  Pos BFS_safety_box(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<Pos, Pos>> q;
    Pos next_step_pos;

    q.push({p, p});
    dist[p.i][p.j] = 0;

    while (!q.empty()) {
      int x = q.front().first.i;
      int y = q.front().first.j;
      Pos step_pos = q.front().second;
      q.pop();

      for (int i = 0; i<4; i++) {
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];

        if (pos_ok(nx, ny)) {
          Cell nc = cell(nx,ny);
          if (nc.type != Wall && dist[nx][ny] == -1) {
            if (p == step_pos) next_step_pos = Pos(nx, ny);
            else next_step_pos = step_pos;

            if (nc.box != -1) return next_step_pos;

            dist[nx][ny] = dist[x][y] + 1;
            q.push({Pos(nx,ny), next_step_pos});
          }
        }
      }
    }
    return p;
  }

  Pos directon_to_box_door(Pos& p) {
    for (int i = 0; i < 4; i++) {
      int nx = p.i + dir_x[i];
      int ny = p.j + dir_y[i];
      if (pos_ok(nx, ny) && cell(nx, ny).box != -1) return Pos(nx, ny);
    }
    return p;
  }

  bool backtrack(int triplet, vector<int>& hint, vector<int>& pass, int n) {
    if (triplet == n) return true;

    for (int i = 0; i < 3*n ; i++) {
      if (pass[i] != -1) continue;//cojer solo posiciones no visitadas
      for (int j = i+1; j < 3*n; j++) {
        if (pass[j] != -1) continue;//cojer solo posiciones no visitadas
        for (int k = j+1; k < 3*n; k++) {
          if (pass[k] != -1) continue;//cojer solo posiciones no visitadas

          int a = hint[i];
          int b = hint[j];
          int c = hint[k];

          //si no cumplen, mirar otras combinaciones
          if (a+b!=c && a+c!=b && b+c!=a) continue;

          //asignar la solucion encontrada
          pass[i] = triplet;
          pass[j] = triplet;
          pass[k] = triplet;

          //si en las siguientes tuplas se encuentran todas las combinaciones,
          //es que hemos encontrado la contraseña
          if (backtrack(triplet+1, hint, pass, n)) return true;

          //si devuelve false es que no hemos encontrado una de las combinaciones que faltan,
          //tenemos que desbloquear las posiciones porque no son una solucion posible
          pass[i] = -1;
          pass[j] = -1;
          pass[k] = -1;
        }
      }
    }
    //no se ha encontrado una posible solucion
    return false;
  }

  vector<int> BF_box_password(int box_id) {//PUEDE PASAR QUE SIGA A OTRO PROFRESOR QUE LE VA DELANTE Y NUNCA COJA NINGUNA CAJA
    vector<int> hint = safety_box_hints(box_id);
    vector<int> pass (hint.size(), -1);
    int n = hint.size() / 3;
    
    backtrack(0, hint, pass, n);
    
    return pass;
  }
  
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    double st = status(me());
    if (st >= 0.9) return;

    int n = board_rows();
    int m = board_cols();

    vector<int> s = soldiers(me());
    for (int i = 0; i < int(s.size()); i++) {
      Unit u = unit(s[i]);
      if (available_poison(me()) > 0 && u.poison < 2) {
        charge(s[i]); //recarga veneno si tiene menos de 2
      } else {
        //hacer que huya si tiene un enemigo cerca y no tiene veneno*******************************************************************
        if (u.poison > 0) {//que vayan a matar solo si tienen veneno
          Pos pos_to_kill = BFS_kill_soldiers_or_professors(u.pos, n, m);//mejorarlo y que se prioricen profresores(?)
          if (pos_to_kill != Pos(-1, -1)) move(u.id, go_to(u.pos, pos_to_kill));
          else move(u.id, Down);
        } else {//los demas van a recoger kits y oro
          Pos pos_to_loot = BFS_take_kits_or_money(u.pos, n, m);
          //Pos pos_to_loot = BFS_take_poison_money(u.pos, n, m);
          if (pos_to_loot != Pos(-1,-1)) move(u.id, go_to(u.pos, pos_to_loot));
          else move(u.id, Down);
        }
      }
    }

    Unit p = unit(professor(me())); //mi profresor
    if (available_health(me()) > 0 && p.health < professor_max_health()) {
      heal(p.id); //se cura si tiene menos del tope de vida
    } else {
      //ir en direccion a una caja fuerte
      Pos box_pos = directon_to_box_door(p.pos);
      if (box_pos != p.pos) {//si esta en la puerta, abrirla
        vector<int> sol = BF_box_password(cell(box_pos).box);//conseguir solución
        open(p.id, sol, go_to(p.pos, box_pos));//abrir caja
      } else {//si no esta en la puerta, ir hacia una
        Pos pos_to_box = BFS_safety_box(p.pos, n, m);
        if (pos_to_box != Pos(-1, -1)) move(p.id, go_to(p.pos, pos_to_box));
        else move(p.id, Down);
      }
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
