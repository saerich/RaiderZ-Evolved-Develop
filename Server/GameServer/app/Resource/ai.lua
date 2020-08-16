--------------------------------------------
-- List utility
--------------------------------------------
List = {}

function List.new ()
	return { first = 0, last = -1}
end

function List.pushleft (list, value)
	local first = list.first-1
	list.first  = first
	list[first] = value;
end

function List.pushright (list, value)
	local last = list.last + 1
	list.last = last
	list[last] = value
end

function List.popleft (list)
	local first = list.first
	if first > list.last then 
		return nil
	end
	local value = list[first]
	list[first] = nil         -- to allow garbage collection
	list.first = first+1
	return value
end

function List.popright (list)
	local last = list.last
	if list.first > last then
		return nil
	end
	local value = list[last]
	list[last] = nil
	list.last = last-1
	return value 
end

function List.clear (list)
	for i,v in ipairs(list) do
		list[i] = nil
	end
--[[
	if List.size(list) == 0 then
		return
	end
	local first = list.first
	local last  = list.last
	for i=first, last do
		list[i] = nil
	end
--]]
	list.first = 0
	list.last = -1
end

function List.size (list)
	local size = list.last - list.first + 1
	return size
end

-------------------------------------------------
MonsterAIMgr = {}

FieldList = {}
FieldMgr = {}

function FieldMgr.Add(fID)
	local MonsterAIList = {}
	FieldList[fID] = MonsterAIList -- ����AI ����Ʈ�� �ִ´�. ������ ����ְ� ���߿� AI�� ������ �� �߰��Ѵ�.
end


--function MonsterAIMgr.Add(fID)	-- ���͸� �߰��Ѵ�.
--	List.pushright(FieldList[fID], gMonsterAI)
--end
function MonsterAIMgr.Add(fID, aID)
	FieldList[fID][aID] = gMonsterAI
end

	

-- �ʿ��� �� ������ ��. ���� "Ư��" AI ��� ���Ű� �ʿ��ϴٸ� AI ����� List�� �ƴ� Vector �������� ��ƾ� ����..
--function MonsterAIManager.Remove(��ƿ��� ������ GModuleAI���� -> �ᱹ GEntityMonster�� MUID��..)
--	MonsterAIList.pop(GEntityMonster::MUID) -- �̷��� �Ǿ����
--end

function MonsterAIMgr.RandMove(fID, aID)
	local ai = FieldList[fID][aID]
	if(ai:IsReady()) then
		local pos = vec3()
		pos = ai:GetPosition()

		--math.randomseed(os.time())
		x = math.random(math.max(pos.x - 1000.0, 50.0), pos.x + 1000.0)
		y = math.random(math.max(pos.y - 1000.0, 50.0), pos.y + 1000.0)
		z = math.random(math.max(pos.z - 1000.0, 50.0), pos.z + 1000.0)
			
		ai:Move(x, y, z, 100.0)
	end
end