#include "Command.h"
#include <algorithm>
using std::cout;
using std::endl;

void Command::next() {
	if (n % 2) {
		if (nowTo.y == 1) {
			if (nowTo.x + 2 <= m) {
				nowTo.x += 2;
				nowTo.y = (n - 1) / 2;
			}
			else {
				std::cerr << "The repository is full!!!" << std::endl;
			}
		}
		else {
			if (nowTo.y <= (n - 1) / 2)
				nowTo.y = n + 2 - nowTo.y;
			else
				nowTo.y = n + 1 - nowTo.y;
		}
	}
	else {
		if (nowTo.y == n) {
			if (nowTo.x + 2 <= m) {
				nowTo.x += 2;
				nowTo.y = (n - 2) / 2;
			}
			else {
				std::cerr << "The repository is full!!!" << std::endl;
				exit(-1);
			}
		}
		else {
			if (nowTo.y <= (n - 2) / 2)
				nowTo.y = n + 1 - nowTo.y;
			else
				nowTo.y = n - nowTo.y;
		}
	}
}

Point Command::nextPlace() {
	if (!spares.empty()) {
		Point ret = spares.back();
		spares.pop_back();
		return ret;
	}
	else {
        Point temp=nowTo;
		next();
		return temp;
	}
}

Point Command::getPoint(int cc) {
	if (places[cc].empty()) {
		std::cerr << cc << ": No such droplets found!!!" << std::endl;
		exit(-1);
	}
	else {
		Point ret = places[cc].back();
		places[cc].pop_back();
		return ret;
	}
}

bool Command::update(Chip& chip)
{
	if(nowStep==steps.size()) return 0;
    StepInfo &step=steps[nowStep];
    nowStep++;
    
	if (step.sample)
	{
		chip.getS = 1;
		chip.SRoute.resize(0);
		Point p = nextPlace();
		if (p.x == 2)
			for (int i = 1; i <= p.y; i++)
				chip.SRoute.push_back({ 1,i });
		else
		{
			for (int i = 1; i <= (n + 1) / 2; i++)
				chip.SRoute.push_back({ 1,i });
			for (int i = 2; i < p.x; i++)
				chip.SRoute.push_back({ i,(n + 1) / 2 });
			if (p.y > (n + 1) / 2)
				for (int i = (n + 1) / 2 + 1; i <= p.y; i++)
					chip.SRoute.push_back({ p.x - 1,i });
			else
				for (int i = (n + 1) / 2 - 1; i >= p.y; i--)
					chip.SRoute.push_back({ p.x - 1,i });
		}
		chip.SRoute.push_back(p);
		places[1 << d].push_back(p);
	}
	else chip.getS = 0;
    if(step.buffer)
    {
        chip.getB=1;
        chip.BRoute.resize(0);
        Point p=nextPlace();
        if(p.x==2)
            for(int i=n;i>=p.y;i--)
                chip.BRoute.push_back({1,i});
        else
        {
            for(int i=n;i>=(n+1)/2+1;i--)
                chip.BRoute.push_back({1,i});
            for(int i=2;i<p.x;i++)
                chip.BRoute.push_back({i,(n+1)/2+1});
            if(p.y>(n+1)/2+1)
                for(int i=(n+1)/2+2;i<=p.y;i++)
                    chip.BRoute.push_back({p.x-1,i});
            else
                for(int i=(n+1)/2;i>=p.y;i--)
                    chip.BRoute.push_back({p.x-1,i});
        }
        chip.BRoute.push_back(p);
		places[0].push_back(p);
    }
    else chip.getB=0;
    
    Point p1=getPoint(step.merge1);
    Point p2=getPoint(step.merge2);
    //places[step.merge1].push_back(p1);
    //places[step.merge2].push_back(p2);
    if(p1.y>p2.y) std::swap(p1,p2);
    vector<Point>& r1=chip.route[0];
    vector<Point>& r2=chip.route[1];
    r1.resize(0);
    r2.resize(0);
    if(std::abs(p1.x-p2.x)<=2) //near enough,meet on midpoint
    {
        int meetx=std::min(p1.x,p2.x)+1;
        r1.push_back(p1);
        r2.push_back(p2);
        for(int i=p1.y;i<=(p1.y+p2.y)/2;i++) r1.push_back({meetx,i});
        for(int i=p2.y;i>=(p1.y+p2.y)/2+1;i--) r2.push_back({meetx,i});
    }
    else
    {
        int meetx=(p1.x+p2.x)/2;
        if(p2.y<=(n+1)/2) //both on left
        {
            if(p1.x>p2.x) std::swap(p1,p2); //p1 upper
            r1.push_back(p1);
            r2.push_back(p2);
            for(int i=p1.y;i<=(n+1)/2;i++) r1.push_back({p1.x+1,i});
            for(int i=p1.x+2;i<=meetx;i++) r1.push_back({i,(n+1)/2});
            for(int i=p2.y;i<=(n+1)/2;i++) r2.push_back({p2.x-1,i});
            for(int i=p2.x-2;i>meetx;i--) r2.push_back({i,(n+1)/2});
        }
        else if(p1.y>(n+1)/2) //both on right
        {
            if(p1.x>p2.x) std::swap(p1,p2); //p1 upper
            r1.push_back(p1);
            r2.push_back(p2);
            for(int i=p1.y;i>(n+1)/2;i--) r1.push_back({p1.x+1,i});
            for(int i=p1.x+2;i<=meetx;i++) r1.push_back({i,(n+1)/2+1});
            for(int i=p2.y;i>(n+1)/2;i--) r2.push_back({p2.x-1,i});
            for(int i=p2.x-2;i>meetx;i--) r2.push_back({i,(n+1)/2+1});
        }
        else //p1 on left and p2 on right
        {
            r1.push_back(p1);
            r2.push_back(p2);
            if(p1.x<meetx) //p1 upper
            {
                for(int i=p1.y;i<=(n+1)/2;i++) r1.push_back({p1.x+1,i});
                for(int i=p1.x+2;i<=meetx;i++) r1.push_back({i,(n+1)/2});
                for(int i=p2.y;i>(n+1)/2;i--) r2.push_back({p2.x-1,i});
                for(int i=p2.x-2;i>meetx;i--) r2.push_back({i,(n+1)/2+1});
            }
            else //p2 upper
            {
                for(int i=p1.y;i<=(n+1)/2;i++) r1.push_back({p1.x-1,i});
                for(int i=p1.x-2;i>meetx;i--) r1.push_back({i,(n+1)/2});
                for(int i=p2.y;i>(n+1)/2;i--) r2.push_back({p2.x+1,i});
                for(int i=p2.x+2;i<=meetx;i++) r2.push_back({i,(n+1)/2+1});
            }
        }
    }
	int target = (step.merge1 + step.merge2) / 2;
	places[target].push_back(p1);
	places[target].push_back(p2);
    if(step.waste!=-1)
    {
		if (step.waste != target) std::cerr << "Something goes wrong!!!" << std::endl;
		Point q=getPoint(step.waste);
		spares.push_back(q);
        chip.discard=1;
        chip.discardRoute.resize(0);
        chip.discardRoute.push_back(q);
        if(q.y<=(n+1)/2)
            for(int i=q.y;i<=(n+1)/2;i++) chip.discardRoute.push_back({q.x-1,i});
        else
            for(int i=q.y;i>=(n+1)/2;i--) chip.discardRoute.push_back({q.x-1,i});
        for(int i=q.x-2;i>=1;i--) chip.discardRoute.push_back({i,(n+1)/2});
    }
    else chip.discard=0;

	for (int r = 1; r <= m; r++)
		for (int c = 1; c <= n; c++)
			chip.map[r][c] = -1;
	for (int i = 0; i <= (1 << d); i++)
		for (Point p : places[i])
			chip.map[p.x][p.y] = i;

    return 1;
}