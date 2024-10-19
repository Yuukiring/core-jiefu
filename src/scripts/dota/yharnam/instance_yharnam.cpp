#include "scriptPCH.h"
#include "yharnam.h"

struct instance_yharnam : public ScriptedInstance
{
    instance_yharnam(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    };

    uint64 m_uiBloodStarvedBeastGUID;
    uint32 m_uiSpawnChestOnAllBossDeath;
    bool   m_isBloodStarvedBeastDead;

    void Initialize() override
    {
        m_uiBloodStarvedBeastGUID = 0;
        m_uiSpawnChestOnAllBossDeath = 10000;
        m_isBloodStarvedBeastDead = false;
    }

    void OnCreatureCreate(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_BLOOD_STARVED_BEAST:
                m_uiBloodStarvedBeastGUID = pCreature->GetGUID();
                break;
        }
    }

    void OnCreatureDeath(Creature *who) override
    {
        switch (who->GetEntry())
        {
            case NPC_BLOOD_STARVED_BEAST :
                m_isBloodStarvedBeastDead = true;
                m_uiSpawnChestOnAllBossDeath = 10000;
                break;
        }
    }

    void Update(uint32 uiDiff) override
    {
        //spawn chest
        if (m_isBloodStarvedBeastDead && m_uiSpawnChestOnAllBossDeath)
        {
            if (m_uiSpawnChestOnAllBossDeath <= uiDiff)
            {
                if (Creature* pBloodStarvedBeast = instance->GetCreature(m_uiBloodStarvedBeastGUID))
                {
                    pBloodStarvedBeast->SummonGameObject(GO_CHEST, -1095.44f, 2234.75f, 182.862f, 0.0f, 0, 0, 0, 0, 43200);
                    m_uiSpawnChestOnAllBossDeath = 0;
                }
            }
            else
                m_uiSpawnChestOnAllBossDeath -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_yharnam(Map* pMap)
{
    return new instance_yharnam(pMap);
}

void AddSC_instance_yharnam()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_yharnam";
    newscript->GetInstanceData = &GetInstanceData_instance_yharnam;
    newscript->RegisterSelf();
}