#include <bits/stdc++.h>
#include <ncurses.h>
#include "Planet.h"
#include "TwoDim.h"
using namespace std;
#define deb(x) cout << #x << ":" << x << "\n"
#define pb push_back
#define ppb pop_back
#define ll long long
#define double long double
#define per(i, a, b) for (ll i = a; i <= b; i++)
#define rep(i, a, b) for (ll i = a; i >= b; i--)
const ll maxn = 1e3 + 15;

vector<vector<TwoDim>> Distances, Forces;
vector<Planet> planets;
vector<TwoDim> markedCells;
ll planets_count, forces_coefficient, velocity_coefficient;
bool flg[maxn][maxn], game1 = 1, game2 = 1;
double planets_density;
ll maxL, maxC, maxR, minR;
mutex mtx;
ll genRand(ll a, ll b)
{
    random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> distrib(a, b);
    return ((distrib(gen)) % (b - a + 1)) + a;
}

void init(ll n, double p, ll fc, ll vc, ll mnr, ll mxr)
{
    vector<TwoDim> tmp;
    per(i, 0, maxn - 1) tmp.pb(TwoDim(0, 0));
    per(i, 0, maxn - 1)
    {
        Forces.pb(tmp);
        Distances.pb(tmp);
    }
    forces_coefficient = fc;
    velocity_coefficient = vc;
    planets.pb(Planet(0, 0, 0, 0, 0, 0, 0, 0));
    getmaxyx(stdscr, maxC, maxL);
    maxL--;
    maxC--;
    maxR = mxr;
    minR = mnr;
    planets_density = p;
    planets_count = n;
    per(i, 1, n)
    {
        bool flg = 1;
        ll randx = genRand(2, maxC - 2);
        ll randy = genRand(2, maxL - 2);
        ll randr = genRand(minR, maxR);
        while (flg)
        {
            flg = 0;
            per(j, 1, i - 1)
            {
                if (randx == planets[j].getCoordinate().x && randy == planets[j].getCoordinate().y)
                {
                    flg = 1;
                    randx = genRand(2, maxC - 2);
                    randy = genRand(2, maxL - 2);
                    break;
                }
            }
            if (!flg)
            {
                if (randx + randr > maxC - 1 || randx - randr < 2 || randy + randr > maxL - 1 || randy - randr < 2)
                {
                    flg = 1;
                    randx = genRand(2, maxC - 2);
                    randy = genRand(2, maxL - 2);
                    randr = genRand(minR, maxR);
                }
            }
        }
        planets.pb(Planet(/*genRand(1,20)*/ 0, /*genRand(1,20)*/ 0, randx, randy, 0, 0, 0, randr));
    }
}

void drawPlanet(ll tmpi, ll tmpj, const Planet &planet)
{
    mvaddch(tmpi, tmpj, '*');
    flg[tmpi][tmpj] = 1;
    markedCells.pb(TwoDim(tmpi, tmpj));
    per(i, -1, 1)
    {
        per(j, -1, 1)
        {

            if (abs(i + j) % 2 == 1 && !flg[tmpi + i][tmpj + j] && sqrt(pow(tmpi + i - planet.getCoordinate().x, 2) + pow(tmpj + j - planet.getCoordinate().y, 2) <= planet.getRadius()))
            {
                drawPlanet(tmpi + i, tmpj + j, planet);
            }
        }
    }
}

void draw()
{
    clear();
    per(i, 0, maxL - 1)
    {
        mvaddch(maxC, i, '#');
        mvaddch(0, i, '#');
    }

    per(i, 0, maxC - 1)
    {
        mvaddch(i, maxL, '#');
        mvaddch(i, 0, '#');
    }
    per(i, 1, planets_count)
    {
        drawPlanet(planets[i].getCoordinate().x, planets[i].getCoordinate().y, planets[i]);
        for (ll j = markedCells.size() - 1; j >= 0; j--)
        {
            flg[(ll)markedCells[j].x][(ll)markedCells[j].y] = 0;
            markedCells.ppb();
        }
    }
    refresh();
}

void setVariables()
{
    per(i, 1, planets_count)
    {
        per(j, i + 1, planets_count)
        {
            ll xi = planets[i].getCoordinate().x;
            ll yi = planets[i].getCoordinate().y;
            ll xj = planets[j].getCoordinate().x;
            ll yj = planets[j].getCoordinate().y;
            double ri = planets[i].getRadius();
            double rj = planets[j].getRadius();
            double mi = pow(ri, 2) * planets_density;
            double mj = pow(rj, 2) * planets_density;
            mi /= forces_coefficient;
            mj /= forces_coefficient;
            ll dxij = xi - xj;
            ll dyij = yi - yj;
            Distances[i][j] = Distances[j][i] = TwoDim(dxij, dyij);
            Forces[i][j] = Forces[j][i] = TwoDim(mi * mj / (dxij * abs(dxij)), mi * mj / (dyij * abs(dyij)));
        }
    }
    per(i, 1, planets_count)
    {
        planets[i].setForce(TwoDim(0, 0));
        per(j, 1, planets_count) if (i != j)
        {
            double Fxi = planets[i].getForce().x;
            double Fyi = planets[i].getForce().y;
            planets[i].setForce(TwoDim(Fxi + Forces[i][j].x, Fyi + Forces[i][j].y));
        }
        
        double mi = pow(planets[i].getRadius(), 2) * planets_density;
        TwoDim vi = planets[i].getVelocity();
        TwoDim xi = planets[i].getCoordinate();
        per(j, 0, maxL - 1)
        {
            double Fxi = planets[i].getForce().x;
            double Fyi = planets[i].getForce().y;
            Fxi -= (mi/forces_coefficient)/((maxC - xi.x) * abs(maxC - xi.x)); 
            Fyi -= (mi/forces_coefficient)/((i - xi.y) * abs(i - xi.y));
            Fxi -= (mi/forces_coefficient)/((0 - xi.x) * abs(0 - xi.x)); 
            Fyi -= (mi/forces_coefficient)/((i - xi.y) * abs(i - xi.y));
            planets[i].setForce(TwoDim(Fxi, Fyi));
        }

        per(j, 0, maxC - 1)
        {
            double Fxi = planets[i].getForce().x;
            double Fyi = planets[i].getForce().y;
            Fxi -= (mi/forces_coefficient)/((i - xi.x) * abs(i - xi.x)); 
            Fyi -= (mi/forces_coefficient)/((maxL - xi.y) * abs(maxL - xi.y));
            Fxi -= (mi/forces_coefficient)/((i - xi.x) * abs(i - xi.x)); 
            Fyi -= (mi/forces_coefficient)/((0 - xi.y) * abs(0 - xi.y));
            planets[i].setForce(TwoDim(Fxi, Fyi));
        }






        TwoDim Fi = planets[i].getForce();
        planets[i].setVelocity(TwoDim(vi.x + (Fi.x / mi) / velocity_coefficient, vi.y + (Fi.y / mi) / velocity_coefficient));
        vi = planets[i].getVelocity();
        planets[i].setCoordinate(TwoDim(xi.x + vi.x, xi.y + vi.y));
    }
}

void readThread()
{
    noecho();
    while (game2)
    {

        auto start = std::chrono::high_resolution_clock::now();
        mtx.lock();
        char c = getch();
        if (c == 's')
        {
            planets[1].setRadius(max(planets[1].getRadius() - 1, (double)minR));
        }
        else if (c == 'w')
        {
            planets[1].setRadius(min(planets[1].getRadius() + 1, (double)maxR));
        }
        else if (c == 'q')
            game1 = game2 = 0;
        mtx.unlock();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        // deb(duration.count());
        this_thread::sleep_for(std::chrono::milliseconds(25 - duration.count()));
    }
}

void performThread()
{
    // deb("perform");
    while (game1)
    {
        mtx.lock();
        setVariables();
        draw();
        mtx.unlock();
        this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

signed main()
{
    game1 = game2 = 1;
    initscr();
    init(1, 5, 10, 10, 3, 10);
    timeout(24);
    // nodelay(stdscr, TRUE);
    thread t1(performThread);
    thread t2(readThread);
    t1.join();
    t2.join();
    endwin();
    return 0;
}
