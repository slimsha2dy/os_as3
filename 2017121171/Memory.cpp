#include "Memory.hpp"

Vmemory::Vmemory()
{
	for (int i = 0; i < 32; ++i)
	{
		this->is_alloc[i] = 0;
		this->pageid[i] = -1;
		this->allocid[i] = -1;
		this->valid[i] = 0;
		this->address[i] = -1;
        this->copied[i] = 0;
	}
}

Vmemory::~Vmemory()
{
}

void	Vmemory::allocVmem(int num, int &allocid, int &pageid)
{
	allocid++;
	pageid++;
	int i = 0;
	int flag = 0;
	// find a space for allocate
	while (i < 32) {
		while (this->is_alloc[i])
			i++;
		for (int j = 0; j < num; ++j) {
			if (this->is_alloc[i + j]) {
				i += j;
				break;
			}
			if (j == num - 1)
				flag = 1;
		}
		if (flag)
			break;
	}
	// allocate
	while (num) {
		this->is_alloc[i] = 1;
		this->pageid[i] = pageid++;
		this->allocid[i] = allocid;
		this->permission[i++] = 1;	// write permission
		--num;
	}
	pageid--;
}

int Vmemory::tableWrite(int allocid, int address)
{
	int i = 0;
	while (i < 32) {
		if (!this->valid[i] && this->allocid[i] == allocid)
			break;
		i++;
	}
	this->address[i] = address;
	this->valid[i] = 1;
	return (this->pageid[i]);
}

bool Vmemory::getIsAlloc(int i) const
{
	return (this->is_alloc[i]);
}

int Vmemory::getPageId(int i) const
{
	return (this->pageid[i]);
}

bool Vmemory::getValid(int i) const
{
	return (this->valid[i]);
}

bool Vmemory::getPermission(int i) const
{
	return (this->permission[i]);
}

int Vmemory::getAddress(int i) const
{
	return (this->address[i]);
}

void	Vmemory::unvalid(int i)
{
	this->valid[i] = 0;
}

int	Vmemory::findAddress(int add) const
{
	int i = 0;
	while (i < 16) {
		if (this->valid[i] && this->address[i] == add)
			return (i);
		i++;
	}
	return (-1);
}

Vmemory& Vmemory::operator=(Vmemory& other)
{
    if (this == &other)
        return *this;
    for (int i = 0; i < 32; ++i) {
        this->is_alloc[i] = other.is_alloc[i];
        this->pageid[i] = other.pageid[i];
        this->allocid[i] = other.allocid[i];
        this->valid[i] = other.valid[i];
        this->address[i] = other.address[i];
        this->permission[i] = 0;
        this->copied[i] = 1;
        other.copied[i] = 1;
    }
    return *this;
}

void Vmemory::permToread(void)
{
    for (int i = 0; i < 32; ++i) {
        this->permission[i] = 0;
    }
}

void Vmemory::permTowrite(int allocid)
{
    for (int i = 0; i < 32; ++i) {
        if (this->is_alloc[i] && this->allocid[i] == allocid)
            this->permission[i] = 1;
    }
}

void Vmemory::permTowritePage(int pageid)
{
    for (int i = 0; i < 32; ++i) {
        if (this->is_alloc[i] && this->pageid[i] == pageid) {
            this->permission[i] = 1;
            return ;
        }
    }
}

vector<int> Vmemory::memoryRelease(int allocid, int pid)
{
    vector<int> paddress;
    for (int i = 0; i < 32; ++i) {
        if (this->is_alloc[i] && this->allocid[i] == allocid) {
            this->is_alloc[i] = 0;
            if (this->valid[i] && (this->permission[i] == 1 || pid == 1))
                paddress.push_back(this->address[i]);
            this->valid[i] = 0;
            this->copied[i] = 0;
        }
    }
    return (paddress);
}

bool Vmemory::getPermofAlloc(int allocid)
{
    for (int i = 0; i < 32; ++i) {
        if (this->is_alloc[i] && this->allocid[i] == allocid && this->copied[i] == 1)
            return (0);
    }
    return (1);
}

int Vmemory::findPageId(int i)
{
    for (int j = 0; j < 32; ++j) {
        if (this->pageid[j] == i)
            return (j);
    }
    return (-1);
}

void Vmemory::changeTable(int pageid, int address)
{
    for (int i = 0; i < 32; ++i) {
        if (this->pageid[i] == pageid) {
            this->address[i] = address;
            this->permission[i] = 1;
            this->valid[i] = 1;
        }
    }
}

int Vmemory::getCopied(int i)
{
    return (this->copied[i]);
}

void Vmemory::setCopied(int i, int j)
{
    this->copied[i] = j;
}
