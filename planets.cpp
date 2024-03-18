#include<bits/stdc++.h>
#include <ncurses.h>
#include "Planet.h"
#include "TwoDim.h"
using namespace std;
#define deb(x) cout<<#x<<":"<<x<<"\n"
#define in(x) cin>>x
#define pb push_back
#define ppb pop_back
#define pii pair<ll, ll>
#define piii pair<ll, pii>
#define f first
#define s second
#define ll long long
#define per(i,a,b) for(ll i=a;i<=b;i++)
#define rep(i,a,b) for(ll i=a;i>=b;i--)
#define outl(x) cout<<x<<endl;
#define outc(x) cout<<x<<" ";
const ll maxn = 1e3 + 15;
const ll mod = 1e9+7;
const ll lg = 22;

vector<vector<TwoDim>> Distances, Forces;
vector<Planet> planets;
vector<TwoDim> markedCells;
int planets_count;
bool flg[maxn][maxn];
double planets_density;
int maxL, maxC, maxR, minR;

int genRand(int a, int b) {
    random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> distrib(a, b);
    return distrib(gen);
}

void init(int n, double p, int mnr, int mxr)
{
    vector<TwoDim> tmp;
    per(i,0,maxn-1) tmp.pb(TwoDim(0, 0));
    per(i,0,maxn-1) { Forces.pb(tmp); Distances.pb(tmp); }
    planets.pb(Planet(0, 0, 0, 0, 0, 0, 0, 0));
    getmaxyx(stdscr, maxL, maxC);
    maxL--; maxC--;
    maxR = mxr; minR = mnr;
    planets_density = p;
    planets_count = n;
    per(i,1,n)
    {
        bool flg = 1;
        int randx = genRand(2,maxL-2);
        int randy = genRand(2,maxC-2);
        int randr = genRand(minR,maxR);
        while(flg)
        {
            flg = 0;
            per(j,1,i-1)
            {
                if(randx == planets[j].getCoordinate().x && randy == planets[j].getCoordinate().y)
                {
                    flg = 1;
                    randx = genRand(2,maxL-2);
                    randy = genRand(2,maxC-2);
                    break;
                }
            }
            if(!flg)
            {
                if(randx + randr > maxL - 1 || randx - randr < 2 || randy + randr > maxC - 1 || randy - randr < 2)
                {
                    flg = 1;
                    randx = genRand(2,maxL-2);
                    randy = genRand(2,maxC-2);
                    randr = genRand(minR,maxR);
                }
            }
        }
        //Planet(double vx, double vy, int cx, int cy, double fx, double fy, int t, double r) :
        //    velocity(vx, vy), coordinate(cx, cy), force(fx, fy), time(t), radius(r) {}

        planets.pb(Planet(/*genRand(1,20)*/0, /*genRand(1,20)*/0, randx, randy, 0, 0, 0, randr));
    }
}

void drawPlanet(int tmpi, int tmpj, const Planet& planet)
{
    mvaddch(tmpi, tmpj, '*');
    flg[tmpi][tmpj] = 1;
    markedCells.pb(TwoDim(tmpi, tmpj));
    per(i,-1,1)
    {
        per(j,-1,1)
        {
            
            if(abs(i+j) % 2 == 1 && !flg[tmpi+i][tmpj+j] && sqrt(pow(tmpi+i - planet.getCoordinate().x, 2) + pow(tmpj+j - planet.getCoordinate().y, 2) <= planet.getRadius()))
            {
                drawPlanet(tmpi+i, tmpj+j, planet);
            }
        }
    }
}

void draw()
{
    clear();
    per(i,1,planets_count)
    {
        drawPlanet(planets[i].getCoordinate().x, planets[i].getCoordinate().y, planets[i]);
        for(int j = markedCells.size()-1; j>=0; j--)
        {
            flg[(int)markedCells[j].x][(int)markedCells[j].y] = 0;
            markedCells.ppb();
        }
    }
    refresh();
}

void setVariables()
{
    per(i,1,planets_count)
    {
        per(j,i+1,planets_count)
        {
            int xi = planets[i].getCoordinate().x;
            int yi = planets[i].getCoordinate().y;
            int xj = planets[j].getCoordinate().x;
            int yj = planets[j].getCoordinate().y;
            double ri = planets[i].getRadius();
            double rj = planets[j].getRadius();
            double mi = (3.14)*pow(ri, 2)*planets_density;
            double mj = (3.14)*pow(rj, 2)*planets_density;
            int dxij = abs(xi - xj);
            int dyij = abs(yi - yj);
            Distances[i][j] = Distances[j][i] = TwoDim(dxij, dyij);
            Forces[i][j] = Forces[j][i] = TwoDim(mi*mj/pow(dxij, 2), mi*mj/pow(dyij, 2));
        }
    }
    per(i,1,planets_count)
    {
        planets[i].setForce(TwoDim(0, 0));
        per(j,1,planets_count) if(i!=j)
        {
            double Fxi = planets[i].getForce().x;
            double Fyi = planets[i].getForce().y;
            planets[i].setForce(TwoDim(Fxi + Forces[i][j].x, Fyi + Forces[i][j].y));
        }
        double mi = (3.14)*pow(planets[i].getRadius(), 2)*planets_density;
        TwoDim Fi = planets[i].getForce();
        TwoDim vi = planets[i].getVelocity();
        TwoDim xi = planets[i].getCoordinate();
        planets[i].setVelocity(TwoDim(vi.x + (Fi.x/mi), vi.y + (Fi.y/mi)));
        //vi = planets[i].getVelocity();
        vi = TwoDim(1, 1);
        deb(planets[i].getVelocity().x);
        deb(planets[i].getVelocity().y);
        planets[i].setCoordinate(TwoDim(xi.x + vi.x, xi.y + vi.y));
        // here
    }

}

signed main()
{    
    initscr();
    init(10, 5, 5, 20);
    bool game = 1;
    //nodelay(stdscr, TRUE);
    timeout(500);
    while(game)
    {
        setVariables();
        draw();
        char c = getch();
        auto end = std::chrono::high_resolution_clock::now();
        if(c == 's') 
        {
            planets[1].setRadius(max(planets[1].getRadius()-1,(double)minR));
        }
        else if(c == 'w')
        {
            planets[1].setRadius(min(planets[1].getRadius()+1,(double)maxR));
        }
        else if(c == 'q') game = 0;
        
    }
    endwin();
    
    return 0;
}