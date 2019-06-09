
class Visualizer;


typedef vector<Point> Path;
class Chip
{
private:
    int m,n,d;
    vector<vector<int> > map;//>=0:concentration -1:none
    Path route[2],BRoute,SRoute,discardRoute;
    bool getB,getS,discard;
    /* data */
public:
    friend class Visualizer;
    Chip(/* args */);
    ~Chip();
};

Chip::Chip(/* args */)
{
}

Chip::~Chip()
{
}
