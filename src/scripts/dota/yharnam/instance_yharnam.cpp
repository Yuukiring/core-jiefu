#include "scriptPCH.h"
#include "yharnam.h"

struct instance_yharnam : public ScriptedInstance
{
    instance_yharnam(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    };

    uint64 m_uiBloodStarvedBeastGUID;
    uint64 m_uiFatherGascoigneGUID;
    uint64 m_uiLudwigGUID;
    uint64 m_uiPudgeGUID;
    uint64 m_uiGehrmanGUID;
    uint64 m_uiMoonPresenceGUID;
    uint32 m_uiSpawnChestOnAllBossDeath;
    bool   m_isBloodStarvedBeastDead;
    bool   m_isFatherGascoigneDead;
    bool   m_isLudwigDead;
    bool   m_isPudgeDead;
    bool   m_isGehrmanDead;

    void Initialize() override
    {
        m_uiBloodStarvedBeastGUID = 0;
        m_uiFatherGascoigneGUID = 0;
        m_uiLudwigGUID = 0;
        m_uiPudgeGUID = 0;
        m_uiGehrmanGUID = 0;
        m_uiMoonPresenceGUID = 0;
        m_uiSpawnChestOnAllBossDeath = 10000;
        m_isBloodStarvedBeastDead = false;
        m_isFatherGascoigneDead = false;
        m_isLudwigDead = false;
        m_isPudgeDead = false;
        m_isGehrmanDead = false;
    }

    void OnCreatureCreate(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_BLOOD_STARVED_BEAST:
                m_uiBloodStarvedBeastGUID = pCreature->GetGUID();
                break;
            case NPC_FATHER_GASCOIGNE:
                m_uiFatherGascoigneGUID = pCreature->GetGUID();
                break;
            case NPC_LUDWIG:
                m_uiLudwigGUID = pCreature->GetGUID();
                break;
            case NPC_PUDGE:
                m_uiPudgeGUID = pCreature->GetGUID();
                break;
            case NPC_GEHRMAN:
                m_uiGehrmanGUID = pCreature->GetGUID();
                break;
            case NPC_MOON_PRESENCE:
                m_uiMoonPresenceGUID = pCreature->GetGUID();
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
            case NPC_FATHER_GASCOIGNE :
                m_isFatherGascoigneDead = true;
                m_uiSpawnChestOnAllBossDeath = 10000;
                break;
            case NPC_LUDWIG :
                m_isLudwigDead = true;
                m_uiSpawnChestOnAllBossDeath = 10000;
                break;
            case NPC_PUDGE :
                m_isPudgeDead = true;
                m_uiSpawnChestOnAllBossDeath = 10000;
                break;
            case NPC_GEHRMAN :
                m_isGehrmanDead = true;
                m_uiSpawnChestOnAllBossDeath = 10000;
                if (Creature* pMoonPresence = instance->GetCreature(m_uiMoonPresenceGUID))
                {
                    DoScriptText(SAY_AGGRO_MOON_PRESENCE, pMoonPresence);
                    pMoonPresence->ForcedDespawn(5000);
                }
                break;
        }
    }

    void Update(uint32 uiDiff) override
    {
        //spawn chest
        if (m_isBloodStarvedBeastDead && m_isFatherGascoigneDead && m_isLudwigDead && m_isPudgeDead && m_isGehrmanDead && m_uiSpawnChestOnAllBossDeath)
        {
            if (m_uiSpawnChestOnAllBossDeath <= uiDiff)
            {
                if (Creature* pGehrman = instance->GetCreature(m_uiGehrmanGUID))
                {
                    pGehrman->SummonGameObject(GO_CHEST, -1095.44f, 2234.75f, 182.862f, 0.0f, 0, 0, 0, 0, 43200);
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