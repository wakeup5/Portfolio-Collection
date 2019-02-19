#include "Item.h"

Item::Item(ITEM_TYPE type, char* name, int main, int sub, int price, char* brief)
{
	_itemType = type;
	_itemName = name;
	_main = main;
	_sub = sub;
	_price = price;
	_brief = brief;
}

//Item::Item(Item& item)
//{
//	_itemType = item.getType();
//	_itemName = item.getName();
//	_main = item.getMain();
//	_sub = item.getSub();
//	_price = item.getPrice();
//	_brief = item.getBrief();
//}


Item::~Item()
{
}

ItemManager::ItemManager()
{
	//_items = new vector < Item > ;

	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����", 12, 10, 100));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "����", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����1", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����2", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����3", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����4", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����5", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����6", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����7", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����8", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����9", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����10", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����11", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����12", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����13", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����14", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����15", 12, 10, 200));
	_items.push_back(*new Item(ITEM_TYPE_WEAPON, "���� ����16", 12, 10, 200));
}

ItemManager::~ItemManager()
{
	//delete _items;
}

Item* ItemManager::randomItem(int level)
{
	int randNum = rand() % _items.size();

	return &_items.at(randNum);
}

vector<Item>* ItemManager::getItems()
{
	return &_items;
}

vector<Item>* ItemManager::getItems(ITEM_TYPE type)
{
	vector<Item>* result = new vector<Item>;

	for (int i = 0; i < _items.size(); i++)
	{
		if (_items[i].getType() == type)
		{
			result->push_back(_items[i]);
		}
	}

	return result;
}

int ItemManager::showItem(vector<Item> itemsP)
{
	//GameDisplay::imageClear();
	//system("cls");
	vector<Item> items;
	vector<Item>::iterator itemIterator, pageStartItem, pageLastItem;
	int selectNum = 0;
	int pageNum = 0, lowPerPage = 10;
	int input = 0;

	while (true)
	{
		GameDisplay::imageClear();
		//system("cls");
		items = itemsP;

		printf("   ������ %d / %d\n\n\n", selectNum + 1, items.size());

		pageNum = (selectNum / lowPerPage) + 1;

		pageStartItem = items.begin() + (pageNum - 1) * lowPerPage;

		if (items.size() < (pageNum * lowPerPage))
		{
			pageLastItem = items.end();
		}
		else
		{
			pageLastItem = items.begin() + (pageNum * lowPerPage);
		}

		int i = 0;
		for (itemIterator = pageStartItem; itemIterator != pageLastItem; itemIterator++, i++)
		{
			if (i == selectNum % lowPerPage)
			{
				printf(" > ");
			}
			else
			{
				printf("   ");
			}
			printf("%s - %s\n   main : %d, sub : %d\n   price : %d\n\n"
				, itemIterator->getName(), itemIterator->getBrief(), itemIterator->getMain(), itemIterator->getSub(), itemIterator->getPrice());
		}

		input = _getch();
		if (input == 224)
		{
			input = _getch();
			if (input == 72)
			{
				if (selectNum > 0) selectNum--;
			}
			else if (input == 80)
			{
				if (selectNum < items.size() - 1) selectNum++;
			}
		}
		else if (input == 13)
		{
			return selectNum;
		}
		else if (input == 27)
		{
			//return -1;
			break;
		}

	}

	return -1;
	//delete items;
}