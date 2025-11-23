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

  //HACER UNO ESPECIFICO PARA SOLDADOS Y OTRO PARA PROFESORES
  //CUANDO SE CRUCAN CON ALGUIEN DE SU EQUIPO O NO TIENEN VENENO PARA MATAR NO SE MUEVEN
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

  Pos BFS_kill_soldiers_or_professors(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<Pos> q;
    q.push(p);
    dist[p.i][p.j] = 0;
    while (!q.empty()) {
      int x = q.front().i;
      int y = q.front().j;
      q.pop();
      Cell c = cell(x,y);
      if (c.id != -1) {
        Unit u = unit(c.id);
        if (u.player != me()) return Pos(x,y);
      }
      for (int i = 0; i<4; i++) {
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];
        Cell nc = cell(nx,ny);
        if (pos_ok(Pos(nx, ny)) && nc.type != Wall && dist[nx][ny] == -1) {
          dist[nx][ny] = dist[x][y] + 1;
          q.push(Pos(nx,ny));
        }
      }
    }
    return Pos(-1,-1);
  }

  Pos BFS_take_kits_or_money(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<Pos> q;
    q.push(p);
    dist[p.i][p.j] = 0;
    while (!q.empty()) {
      int x = q.front().i;
      int y = q.front().j;
      q.pop();
      Cell c = cell(x,y);
      if (c.health_kit || c.poison_kit || c.money>0) return Pos(x,y);
      for (int i = 0; i<4; i++) {
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];
        Cell nc = cell(nx,ny);
        if (pos_ok(Pos(nx, ny)) && nc.type != Wall && dist[nx][ny] == -1) {
          dist[nx][ny] = dist[x][y] + 1;
          q.push(Pos(nx,ny));
        }
      }
    }
    return Pos(-1,-1);
  }

  Pos BFS_safety_box(Pos& p, int n, int m) {
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<Pos> q;
    q.push(p);
    dist[p.i][p.j] = 0;
    while (!q.empty()) {
      int x = q.front().i;
      int y = q.front().j;
      q.pop();
      Cell c = cell(x,y);
      if (c.box != -1) return Pos(x,y);
      for (int i = 0; i<4; i++) {
        int nx = x + dir_x[i];
        int ny = y + dir_y[i];
        Cell nc = cell(nx,ny);
        if (pos_ok(Pos(nx, ny)) && nc.type != Wall && dist[nx][ny] == -1) {
          dist[nx][ny] = dist[x][y] + 1;
          q.push(Pos(nx,ny));
        }
      }
    }
    // for (int i = 0; i<n; i++) {
    //   for (int j = 0; j<m; j++) {
    //     cerr << dist[i][j] << " ";
    //   }
    //   cerr << endl;
    // }
    // cerr << endl;
    return Pos(-1,-1);
  }
  
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    double st = status(me());
    if (st >= 0.9) return;

    int n = board_rows();
    int m = board_cols();

    //MAPA PARA PONER LAS DISTANCAS HASTA EL TESORO MAS CERCANO, PARA PODER SEGUIR LAS DIRECCIONES
    //IRSE MOVIENDO DE UN NUMERO MAS ALTO (MAS LEJOS DEL TESORO) HASTA EL MAS PEQUEÑO (MAS CERCA DEL TESORO)
    //0   1   2
    //1   -1  3
    //0 -> TIENE EL TESORO, -1 -> ES UN MURO, LOS DEMAS LA DISTANCIA HASTA EL TESORO
    vector<vector<char>> map (n, vector<char>(m));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        Cell c = cell(i, j);
        if (c.type == Wall) map[i][j] = '-1';
        else if (c.health_kit || c.poison_kit || c.money>0) map[i][j] = '0';
        else if (c.box!=-1) map[i][j] = '0';
      }
    }

    vector<int> s = soldiers(me());
    for (int i = 0; i < int(s.size()); i++) {
      Unit u = unit(s[i]);
      if (available_poison(me()) > 2 && u.poison < 2) charge(s[i]); //recarga veneno si tiene menos de 2
      
      if (random(0,9)<7) {//70% van a matar
        Pos pos_to_kill = BFS_kill_soldiers_or_professors(u.pos, n, m);
        if (pos_to_kill != Pos(-1, -1)) move(u.id, go_to(u.pos, pos_to_kill));
        else move(u.id, Down);
      } else {//30% van a recoger kits y oro
        Pos pos_to_loot = BFS_take_kits_or_money(u.pos, n, m);
        if (pos_to_loot != Pos(-1,-1)) move(u.id, go_to(u.pos, pos_to_loot));
        else move(u.id, Down);
      }
    }

    Unit p = unit(professor(me())); //mi profresor
    if (available_health(me()) > 0 && p.health < 30) heal(p.id); //se cura si tiene menos de 30 de vida
    //ir en direccion a una caja fuerte
    Pos pos_to_box = BFS_safety_box(p.pos, n, m);
    if (pos_to_box != Pos(-1, -1)) move(p.id, go_to(p.pos, pos_to_box));
    else move(p.id, Down);
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
