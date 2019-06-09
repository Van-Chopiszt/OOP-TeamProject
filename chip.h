
class Visualizer;

typedef vector<pair<int,int> > Path;
class Chip
{
private:
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
