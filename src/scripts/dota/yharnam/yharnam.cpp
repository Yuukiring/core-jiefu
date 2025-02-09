#include "scriptPCH.h"
#include "yharnam.h"

enum
{
    EMOTE_ENRAGE    = 2384,
    SPELL_ENRAGE                = 34234,
    //SPELL_TRANSFUR_PATIENT
    SPELL_PIERCE_ARMOR          = 12097,
    SPELL_TERRIFYING_SCREECH    = 6605,
    //SPELL_BLOODWOLF
    SPELL_RAVENOUS_CLAW_1       = 17470,
    SPELL_REND                  = 21949,
    //SPELL_HEMOTHERAPY_PATIENT
    SPELL_RAVENOUS_CLAW_2       = 34235,
    SPELL_DEAFENING_SCREECH     = 34236,
    //SPELL_YHARNAM_CITIZEN
    SPELL_SUNDER_ARMOR          = 11971,
    SPELL_KICK                  = 11978,
    //SPELL_YHARNAM_GUARD
    SPELL_SHIELD_CHARGE         = 15749,
    SPELL_SHIELD_SLAM           = 34237,
    SPELL_DISARM                = 8379,
    //SPELL_YHARNAM_HUNTER
    SPELL_THUNDER_CLAP          = 34238,
    SPELL_INTIMIDATING_SHOUT    = 19134,
    //SPELL_YHARNAM_MEDIC
    SPELL_HEAL                  = 34239,
    SPELL_POWER_WORD_SHIELD     = 34240,
    SPELL_HOLY_FIRE             = 34241,
    //SPELL_BLOOD_STARVED_BEAST
    SPELL_PUNGENT_BLOOD_COCKTAIL    = 34242,
    SPELL_IMPACT                    = 34245,
    SPELL_BLOODTHIRST               = 34246,
    SPELL_CONFUSE                   = 34248,
    SPELL_DRUNKEN                   = 34249,
    //SPELL_FATHER_GASCOIGNE
    SPELL_ANTIGUN                   = 34250,
    SPELL_DEMORALIZING_SHOUT        = 34251,
    SPELL_WHIRLWIND                 = 34252,
    SPELL_WHIRLWIND_AURA            = 34253,
    SPELL_TRANSFUR                  = 34255,
    SPELL_TRANSFUR_CHARM            = 34257,
    //SPELL_LUDWIG
    SPELL_CHARGE_20_P1              = 34258,
    SPELL_CHARGE_30_P1              = 34259,
    SPELL_CHARGE_40_P1              = 34260,
    SPELL_STOMP_P1                  = 34261,
    SPELL_THORNS_P1                 = 34262,
    SPELL_DOUBLE_EDGED_SWORD_P2     = 34263,
    SPELL_STOMP_P2                  = 34264,
    SPELL_THORNS_P2                 = 34265,
    //SPELL_PUDGE
    SPELL_POISONOUS_CLOUD           = 28240,
    SPELL_DISMEMBER                 = 34267,
    SPELL_ROT                       = 34269,
    //SPELL_GEHRMAN
    SPELL_BLOODBORNE                = 34271,
    SPELL_ANTIGUN_GEHRMAN           = 34273,
    SPELL_BLOOD_MOON                = 34274,
    SPELL_VISION                    = 34275,
    //SAY
    SAY_AGGRO_BLOOD_STARVED_BEAST           = -2000013,
    SAY_AGGRO_THE_HUNTER                    = -2000014,
    SAY_AGGRO_PUDGE                         = -2000015,
    SAY_AGGRO_THE_FIRST_HUNTER              = -2000016,
    SAY_AGGRO_LUDWIG_THE_HOLY_BLADE         = -2000018,
    SAY_TRANSITION_THE_HUNTER               = -2000019,
    SAY_DEATH_THE_HUNTER                    = -2000020,
    SAY_DEATH_THE_FIRST_HUNTER              = -2000021,
    SAY_TRANSITION_LUDWIG_THE_HOLY_BLADE    = -2000022,
    SAY_DEATH_LUDWIG_THE_HOLY_BLADE         = -2000023,
    SAY_DEATH_YHARNAM_COMMON                = -2000024,
    SAY_DRUNK_BLOOD_STARVED_BEAST           = -2000025,
    SAY_CONFUSED_BLOOD_STARVED_BEAST        = -2000026,
};

//npc_545_transfur_patient
struct Npc_TransfurPatientAI : public ScriptedAI
{
    Npc_TransfurPatientAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 PIERCE_ARMOR_TIMER;
    uint32 TERRIFYING_SCREECH_TIMER;
    bool HasFled;

    void Reset() override
    {
        PIERCE_ARMOR_TIMER = 2500;
        TERRIFYING_SCREECH_TIMER = 10000;
        HasFled = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(10.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //FLEE
        if (!HasFled && m_creature->GetHealthPercent() < 15.0f)
        {
            HasFled = true;
            DoScriptText(SAY_DEATH_YHARNAM_COMMON, m_creature);
            m_creature->DoFlee();
            return;
        }
        //PIERCE_ARMOR
        if (PIERCE_ARMOR_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_PIERCE_ARMOR);
            PIERCE_ARMOR_TIMER = urand(12500,17500);
        }
        else PIERCE_ARMOR_TIMER -= uiDiff;
        //TERRIFYING_SCREECH
        if (TERRIFYING_SCREECH_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
            {
                if (!pTarget->HasAura(SPELL_TERRIFYING_SCREECH))
                    DoCastSpellIfCan(pTarget, SPELL_TERRIFYING_SCREECH);
                TERRIFYING_SCREECH_TIMER = urand(10000,15000);
            }
        }
        else TERRIFYING_SCREECH_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_TransfurPatientAI(Creature* pCreature)
{
    return new Npc_TransfurPatientAI(pCreature);
}

//npc_545_npc_545_bloodwolf
struct Npc_BloodwolfAI : public ScriptedAI
{
    Npc_BloodwolfAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 RAVENOUS_CLAW_1_TIMER;
    uint32 REND_TIMER;
    bool HasEnraged;

    void Reset() override
    {
        RAVENOUS_CLAW_1_TIMER = 2500;
        REND_TIMER = 7500;
        HasEnraged = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(15.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //ENRAGE
        if (!HasEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            HasEnraged = true;
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            DoScriptText(EMOTE_ENRAGE, m_creature);
        }
        //RAVENOUS_CLAW_1
        if (RAVENOUS_CLAW_1_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_RAVENOUS_CLAW_1);
            RAVENOUS_CLAW_1_TIMER = urand(5000,10000);
        }
        else RAVENOUS_CLAW_1_TIMER -= uiDiff;
        //REND
        if (REND_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
            {
                if (!pTarget->HasAura(SPELL_REND))
                    DoCastSpellIfCan(pTarget, SPELL_REND);
                REND_TIMER = urand(10000,15000);
            }
        }
        else REND_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_BloodwolfAI(Creature* pCreature)
{
    return new Npc_BloodwolfAI(pCreature);
}

//npc_545_hemotherapy_patient
struct Npc_HemotherapyPatientAI : public ScriptedAI
{
    Npc_HemotherapyPatientAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 RAVENOUS_CLAW_2_TIMER;
    uint32 DEAFENING_SCREECH_TIMER;
    bool HasEnraged;

    void Reset() override
    {
        RAVENOUS_CLAW_2_TIMER = 2500;
        DEAFENING_SCREECH_TIMER = 10000;
        HasEnraged = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(10.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //ENRAGE
        if (!HasEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            HasEnraged = true;
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            DoScriptText(EMOTE_ENRAGE, m_creature);
        }
        //RAVENOUS_CLAW_2
        if (RAVENOUS_CLAW_2_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_RAVENOUS_CLAW_2);
            RAVENOUS_CLAW_2_TIMER = urand(5000,10000);
        }
        else RAVENOUS_CLAW_2_TIMER -= uiDiff;
        //DEAFENING_SCREECH
        if (DEAFENING_SCREECH_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
            {
                if (!pTarget->HasAura(SPELL_DEAFENING_SCREECH))
                    DoCastSpellIfCan(pTarget, SPELL_DEAFENING_SCREECH);
                DEAFENING_SCREECH_TIMER = urand(12500,17500);
            }
        }
        else DEAFENING_SCREECH_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_HemotherapyPatientAI(Creature* pCreature)
{
    return new Npc_HemotherapyPatientAI(pCreature);
}

//npc_545_yharnam_citizen
struct Npc_YharnamCitizenAI : public ScriptedAI
{
    Npc_YharnamCitizenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 SUNDER_ARMOR_TIMER;
    uint32 KICK_TIMER;
    bool HasFled;

    void Reset() override
    {
        SUNDER_ARMOR_TIMER = 2500;
        KICK_TIMER = 7500;
        HasFled = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(15.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //FLEE
        if (!HasFled && m_creature->GetHealthPercent() < 15.0f)
        {
            HasFled = true;
            DoScriptText(SAY_DEATH_YHARNAM_COMMON, m_creature);
            m_creature->DoFlee();
            return;
        }
        //SUNDER_ARMOR
        if (SUNDER_ARMOR_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SUNDER_ARMOR);
            SUNDER_ARMOR_TIMER = urand(5000,10000);
        }
        else SUNDER_ARMOR_TIMER -= uiDiff;
        //KICK
        if (KICK_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_KICK);
            KICK_TIMER = urand(7500,12500);
        }
        else KICK_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_YharnamCitizenAI(Creature* pCreature)
{
    return new Npc_YharnamCitizenAI(pCreature);
}

//npc_545_yharnam_guard
struct Npc_YharnamGuardAI : public ScriptedAI
{
    Npc_YharnamGuardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 SHIELD_CHARGE_TIMER;
    uint32 SHIELD_SLAM_TIMER;
    uint32 DISARM_TIMER;
    bool HasEnraged;

    void Reset() override
    {
        SHIELD_CHARGE_TIMER = 500;
        SHIELD_SLAM_TIMER = 5000;
        DISARM_TIMER = 10000;
        HasEnraged = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(10.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //ENRAGE
        if (!HasEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            HasEnraged = true;
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            DoScriptText(EMOTE_ENRAGE, m_creature);
        }
        //SHIELD_CHARGE
        if (SHIELD_CHARGE_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
            {
                float shield_charge_distance = m_creature->GetDistance(pTarget);
                if (shield_charge_distance >= 8.0f && shield_charge_distance <= 25.0f)
                    DoCastSpellIfCan(pTarget, SPELL_SHIELD_CHARGE);
                SHIELD_CHARGE_TIMER = urand(5000,7500);
            }
        }
        else SHIELD_CHARGE_TIMER -= uiDiff;
        //SHIELD_SLAM
        if (SHIELD_SLAM_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SHIELD_SLAM);
            SHIELD_SLAM_TIMER = urand(7500,12500);
        }
        else SHIELD_SLAM_TIMER -= uiDiff;
        //DISARM
        if (DISARM_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
            {
                if (!pTarget->HasAura(SPELL_DISARM))
                    DoCastSpellIfCan(pTarget, SPELL_DISARM);
                DISARM_TIMER = urand(15000,20000);
            }
        }
        else DISARM_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_YharnamGuardAI(Creature* pCreature)
{
    return new Npc_YharnamGuardAI(pCreature);
}

//npc_545_yharnam_hunter
struct Npc_YharnamHunterAI : public ScriptedAI
{
    Npc_YharnamHunterAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 THUNDER_CLAP_TIMER;
    uint32 INTIMIDATING_SHOUT_TIMER;
    bool HasEnraged;

    void Reset() override
    {
        THUNDER_CLAP_TIMER = 5000;
        INTIMIDATING_SHOUT_TIMER = 10000;
        HasEnraged = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(10.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //ENRAGE
        if (!HasEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            HasEnraged = true;
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            DoScriptText(EMOTE_ENRAGE, m_creature);
        }
        //THUNDER_CLAP
        if (THUNDER_CLAP_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_THUNDER_CLAP);
            THUNDER_CLAP_TIMER = urand(7500,12500);
        }
        else THUNDER_CLAP_TIMER -= uiDiff;
        //INTIMIDATING_SHOUT
        if (INTIMIDATING_SHOUT_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
            {
                if (!pTarget->HasAura(SPELL_INTIMIDATING_SHOUT))
                    DoCastSpellIfCan(pTarget, SPELL_INTIMIDATING_SHOUT);
                INTIMIDATING_SHOUT_TIMER = urand(15000,20000);
            }
        }
        else INTIMIDATING_SHOUT_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_YharnamHunterAI(Creature* pCreature)
{
    return new Npc_YharnamHunterAI(pCreature);
}

//npc_545_yharnam_medic
struct Npc_YharnamMedicAI : public ScriptedAI
{
    Npc_YharnamMedicAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 POWER_WORD_SHIELD_TIMER;
    uint32 HEAL_TIMER;
    uint32 HOLY_FIRE_TIMER;
    bool HasFled;

    void Reset() override
    {
        POWER_WORD_SHIELD_TIMER = 7500;
        HEAL_TIMER = 5000;
        HOLY_FIRE_TIMER = 2500;
        HasFled = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(10.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //FLEE
        if (!HasFled && m_creature->GetHealthPercent() < 15.0f)
        {
            HasFled = true;
            DoScriptText(SAY_DEATH_YHARNAM_COMMON, m_creature);
            m_creature->DoFlee();
            return;
        }
        //POWER_WORD_SHIELD
        if (POWER_WORD_SHIELD_TIMER < uiDiff)
        {
            if (Unit* pFriend = m_creature->FindLowestHpFriendlyUnit(40.0f, 30, true))
            {
                if (!pFriend->HasAura(SPELL_POWER_WORD_SHIELD))
                    DoCastSpellIfCan(pFriend, SPELL_POWER_WORD_SHIELD);
                POWER_WORD_SHIELD_TIMER = urand(7500,12500);
            }
        }
        else POWER_WORD_SHIELD_TIMER -= uiDiff;
        //HEAL
        if (HEAL_TIMER < uiDiff)
        {
            if (Unit* pFriend = m_creature->FindLowestHpFriendlyUnit(40.0f, 15, true))
            {
                DoCastSpellIfCan(pFriend, SPELL_HEAL);
                HEAL_TIMER = urand(5000,7500);
            }
        }
        else HEAL_TIMER -= uiDiff;
        //HOLY_FIRE
        if (HOLY_FIRE_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_HOLY_FIRE);
            HOLY_FIRE_TIMER = urand(5000,7500);
        }
        else HOLY_FIRE_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_YharnamMedicAI(Creature* pCreature)
{
    return new Npc_YharnamMedicAI(pCreature);
}

//boss_bloodstarvedbeast
struct Boss_BloodStarvedBeast : public ScriptedAI
{
    Boss_BloodStarvedBeast(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 IMPACT_TIMER;
    bool bloodthirst_90;
    bool bloodthirst_50;
    bool bloodthirst_10;

    void Reset() override
    {
        IMPACT_TIMER = 7500;
        bloodthirst_90 = false;
        bloodthirst_50 = false;
        bloodthirst_10 = false;
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO_BLOOD_STARVED_BEAST, m_creature);
        m_creature->CallForHelp(90.0f);
    }

    void AssignRandomThreat()
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER))
        {
            DoResetThreat();
            m_creature->GetThreatManager().addThreatDirectly(pTarget, urand(1000, 2000));
        }
    }

    void SpellHit(SpellCaster* /*pCaster*/, SpellEntry const* pSpell) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (pSpell->Id == SPELL_PUNGENT_BLOOD_COCKTAIL)
        {
            if (m_creature->HasAura(SPELL_BLOODTHIRST))
            {
                m_creature->RemoveAurasDueToSpell(SPELL_BLOODTHIRST);
                DoCastSpellIfCan(m_creature, SPELL_DRUNKEN);
                DoScriptText(SAY_DRUNK_BLOOD_STARVED_BEAST, m_creature);
            }
            else
            {
                AssignRandomThreat();
                DoCastSpellIfCan(m_creature, SPELL_CONFUSE);
                DoScriptText(SAY_CONFUSED_BLOOD_STARVED_BEAST, m_creature);
            }
        }
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_creature->GetHealthPercent() < 90.0f && !bloodthirst_90)
        {
            if (m_creature->HasAura(SPELL_CONFUSE))
                m_creature->RemoveAurasDueToSpell(SPELL_CONFUSE);
            if (m_creature->HasAura(SPELL_DRUNKEN))
                m_creature->RemoveAurasDueToSpell(SPELL_DRUNKEN);
            DoCastSpellIfCan(m_creature, SPELL_BLOODTHIRST);
            DoScriptText(EMOTE_ENRAGE, m_creature);
            bloodthirst_90 = true;
        }

        if (m_creature->GetHealthPercent() < 50.0f && !bloodthirst_50)
        {
            if (m_creature->HasAura(SPELL_CONFUSE))
                m_creature->RemoveAurasDueToSpell(SPELL_CONFUSE);
            if (m_creature->HasAura(SPELL_DRUNKEN))
                m_creature->RemoveAurasDueToSpell(SPELL_DRUNKEN);
            DoCastSpellIfCan(m_creature, SPELL_BLOODTHIRST);
            DoScriptText(EMOTE_ENRAGE, m_creature);
            bloodthirst_50 = true;
        }

        if (m_creature->GetHealthPercent() < 10.0f && !bloodthirst_10)
        {
            if (m_creature->HasAura(SPELL_CONFUSE))
                m_creature->RemoveAurasDueToSpell(SPELL_CONFUSE);
            if (m_creature->HasAura(SPELL_DRUNKEN))
                m_creature->RemoveAurasDueToSpell(SPELL_DRUNKEN);
            DoCastSpellIfCan(m_creature, SPELL_BLOODTHIRST);
            DoScriptText(EMOTE_ENRAGE, m_creature);
            bloodthirst_10 = true;
        }

        //IMPACT
        if (IMPACT_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_IMPACT);
            IMPACT_TIMER = urand(12500,17500);
        }
        else IMPACT_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Boss_BloodStarvedBeast(Creature* pCreature)
{
    return new Boss_BloodStarvedBeast(pCreature);
}

//boss_father_gascoigne
struct Boss_FatherGascoigne : public ScriptedAI
{
    Boss_FatherGascoigne(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 ANTIGUN_TIMER;
    uint32 DEMORALIZING_SHOUT_TIMER;
    uint32 WHIRLWIND_TIMER;
    uint32 TRANSFUR_CHARM_TIMER;
    bool whirlwind_aura_80;
    bool whirlwind_aura_60;
    bool transfur_40;

    void Reset() override
    {
        ANTIGUN_TIMER = 1000;
        DEMORALIZING_SHOUT_TIMER = 10000;
        WHIRLWIND_TIMER = 5000;
        TRANSFUR_CHARM_TIMER = 7500;
        whirlwind_aura_80 = false;
        whirlwind_aura_60 = false;
        transfur_40 = false;
        m_creature->LoadEquipment(m_creature->GetCreatureInfo()->equipment_id, true);
        m_creature->SetDisplayId(16012);
    }

    void JustDied(Unit* Killer) override
    {
        DoScriptText(SAY_DEATH_THE_HUNTER, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO_THE_HUNTER, m_creature);
        m_creature->CallForHelp(90.0f);
    }

    void AssignRandomThreat()
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER))
        {
            DoResetThreat();
            m_creature->GetThreatManager().addThreatDirectly(pTarget, urand(1000, 2000));
        }
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_creature->GetHealthPercent() < 80.0f && !whirlwind_aura_80)
        {
            DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND_AURA);
            AssignRandomThreat();
            whirlwind_aura_80 = true;
        }

        if (m_creature->GetHealthPercent() < 60.0f && !whirlwind_aura_60)
        {
            DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND_AURA);
            AssignRandomThreat();
            whirlwind_aura_60 = true;
        }

        if (m_creature->GetHealthPercent() < 40.0f && !transfur_40)
        {
            DoCastSpellIfCan(m_creature, SPELL_TRANSFUR);
            AssignRandomThreat();
            transfur_40 = true;
            m_creature->SetVirtualItem(BASE_ATTACK, 0);
            m_creature->SetVirtualItem(OFF_ATTACK, 0);
            m_creature->SetVirtualItem(RANGED_ATTACK, 0);
            m_creature->SetDisplayId(11179);
            DoScriptText(SAY_TRANSITION_THE_HUNTER, m_creature);
        }

        if (!m_creature->HasAura(SPELL_WHIRLWIND_AURA) && !transfur_40)
        {
            //ANTIGUN
            if (ANTIGUN_TIMER < uiDiff)
            {
                if (Unit* pTarget = m_creature->GetVictim())
                {
                    float shield_charge_distance = m_creature->GetDistance(pTarget);
                    if (shield_charge_distance >= 8.0f && shield_charge_distance <= 40.0f)
                    {
                        DoCastSpellIfCan(pTarget, SPELL_ANTIGUN);
                        ANTIGUN_TIMER = urand(5500,6500);
                    }
                }
            }
            else ANTIGUN_TIMER -= uiDiff;

            //DEMORALIZING_SHOUT
            if (DEMORALIZING_SHOUT_TIMER < uiDiff)
            {
                DoCastSpellIfCan(m_creature->GetVictim(), SPELL_DEMORALIZING_SHOUT);
                DEMORALIZING_SHOUT_TIMER = urand(25000,35000);
            }
            else DEMORALIZING_SHOUT_TIMER -= uiDiff;

            //WHIRLWIND
            if (WHIRLWIND_TIMER < uiDiff)
            {
                DoCastSpellIfCan(m_creature->GetVictim(), SPELL_WHIRLWIND);
                WHIRLWIND_TIMER = urand(7500,12500);
            }
            else WHIRLWIND_TIMER -= uiDiff;
        }

        if (transfur_40)
        {
            //TRANSFUR_CHARM
            if (TRANSFUR_CHARM_TIMER < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER))
                {
                    DoCastSpellIfCan(pTarget, SPELL_TRANSFUR_CHARM);
                    TRANSFUR_CHARM_TIMER = urand(17500,22500);
                }
            }
            else TRANSFUR_CHARM_TIMER -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Boss_FatherGascoigne(Creature* pCreature)
{
    return new Boss_FatherGascoigne(pCreature);
}

//boss_ludwig
struct Boss_Ludwig : public ScriptedAI
{
    Boss_Ludwig(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 STOMP_TIMER;
    uint32 THORNS_TIMER;
    bool charge_90;
    bool charge_75;
    bool charge_60;
    bool transition_50;
    bool double_edged_sword_45;
    bool double_edged_sword_30;
    bool double_edged_sword_15;

    void Reset() override
    {
        STOMP_TIMER = 10000;
        THORNS_TIMER = 1000;
        charge_90 = false;
        charge_75 = false;
        charge_60 = false;
        transition_50 = false;
        double_edged_sword_45 = false;
        double_edged_sword_30 = false;
        double_edged_sword_15 = false;
        m_creature->LoadEquipment(m_creature->GetCreatureInfo()->equipment_id, true);
        m_creature->SetDisplayId(9418);
    }

    void JustDied(Unit* Killer) override
    {
        DoScriptText(SAY_DEATH_LUDWIG_THE_HOLY_BLADE, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO_LUDWIG_THE_HOLY_BLADE, m_creature);
        m_creature->CallForHelp(90.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_creature->GetHealthPercent() < 90.0f && !charge_90 && !transition_50)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_FARTHEST, 0, nullptr, SELECT_FLAG_PLAYER))
            {
                float charge_distance = m_creature->GetDistance(pTarget);
                if (charge_distance <= 20.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_20_P1);
                }
                else if (charge_distance > 20.0f && charge_distance <= 30.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_30_P1);
                }
                else if (charge_distance > 30.0f && charge_distance <= 40.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_40_P1);
                }
                else if (charge_distance > 40.0f)
                {
                    DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_NEAREST, 0, nullptr, SELECT_FLAG_PLAYER), SPELL_CHARGE_40_P1);
                }
            }
            charge_90 = true;
        }

        if (m_creature->GetHealthPercent() < 75.0f && !charge_75 && !transition_50)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_FARTHEST, 0, nullptr, SELECT_FLAG_PLAYER))
            {
                float charge_distance = m_creature->GetDistance(pTarget);
                if (charge_distance <= 20.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_20_P1);
                }
                else if (charge_distance > 20.0f && charge_distance <= 30.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_30_P1);
                }
                else if (charge_distance > 30.0f && charge_distance <= 40.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_40_P1);
                }
                else if (charge_distance > 40.0f)
                {
                    DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_NEAREST, 0, nullptr, SELECT_FLAG_PLAYER), SPELL_CHARGE_40_P1);
                }
            }
            charge_75 = true;
        }

        if (m_creature->GetHealthPercent() < 60.0f && !charge_60 && !transition_50)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_FARTHEST, 0, nullptr, SELECT_FLAG_PLAYER))
            {
                float charge_distance = m_creature->GetDistance(pTarget);
                if (charge_distance <= 20.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_20_P1);
                }
                else if (charge_distance > 20.0f && charge_distance <= 30.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_30_P1);
                }
                else if (charge_distance > 30.0f && charge_distance <= 40.0f)
                {
                    DoCastSpellIfCan(pTarget, SPELL_CHARGE_40_P1);
                }
                else if (charge_distance > 40.0f)
                {
                    DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_NEAREST, 0, nullptr, SELECT_FLAG_PLAYER), SPELL_CHARGE_40_P1);
                }
            }
            charge_60 = true;
        }

        if (m_creature->GetHealthPercent() < 50.0f && !transition_50)
        {
            transition_50 = true;
            m_creature->SetVirtualItem(BASE_ATTACK, 19854);
            m_creature->SetVirtualItem(OFF_ATTACK, 0);
            m_creature->SetVirtualItem(RANGED_ATTACK, 0);
            m_creature->SetDisplayId(12373);
            DoScriptText(SAY_TRANSITION_LUDWIG_THE_HOLY_BLADE, m_creature);
        }

        if (m_creature->GetHealthPercent() < 45.0f && !double_edged_sword_45 && transition_50)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_DOUBLE_EDGED_SWORD_P2);
            double_edged_sword_45 = true;
        }

        if (m_creature->GetHealthPercent() < 30.0f && !double_edged_sword_30 && transition_50)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_DOUBLE_EDGED_SWORD_P2);
            double_edged_sword_30 = true;
        }

        if (m_creature->GetHealthPercent() < 15.0f && !double_edged_sword_15 && transition_50)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_DOUBLE_EDGED_SWORD_P2);
            double_edged_sword_15 = true;
        }

        if (!transition_50)
        {
            //STOMP_P1
            if (STOMP_TIMER < uiDiff)
            {
                DoCastSpellIfCan(m_creature->GetVictim(), SPELL_STOMP_P1);
                STOMP_TIMER = urand(25000,35000);
            }
            else STOMP_TIMER -= uiDiff;

            //THORNS_P1
            if (THORNS_TIMER < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_THORNS_P1);
                THORNS_TIMER = urand(15000,25000);
            }
            else THORNS_TIMER -= uiDiff;
        }
        else
        {
            //STOMP_P2
            if (STOMP_TIMER < uiDiff)
            {
                DoCastSpellIfCan(m_creature->GetVictim(), SPELL_STOMP_P2);
                STOMP_TIMER = urand(25000,35000);
            }
            else STOMP_TIMER -= uiDiff;

            //THORNS_P2
            if (THORNS_TIMER < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_THORNS_P2);
                THORNS_TIMER = urand(15000,25000);
            }
            else THORNS_TIMER -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Boss_Ludwig(Creature* pCreature)
{
    return new Boss_Ludwig(pCreature);
}

//boss_pudge
struct Boss_Pudge : public ScriptedAI
{
    Boss_Pudge(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 DISMEMBER_TIMER;
    bool poisonous_cloud_80;
    bool poisonous_cloud_55;
    bool poisonous_cloud_30;
    bool poisonous_cloud_5;

    void Reset() override
    {
        DISMEMBER_TIMER = 10000;
        poisonous_cloud_80 = false;
        poisonous_cloud_55 = false;
        poisonous_cloud_30 = false;
        poisonous_cloud_5 = false;
        if (m_creature->HasAura(SPELL_ROT))
            m_creature->RemoveAurasDueToSpell(SPELL_ROT);
    }

    void Aggro(Unit* pWho) override
    {
        DoCastSpellIfCan(m_creature, SPELL_ROT);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //POISONOUS_CLOUD
        if (m_creature->GetHealthPercent() < 80.0f && !poisonous_cloud_80)
        {
            DoCastSpellIfCan(m_creature, SPELL_POISONOUS_CLOUD);
            poisonous_cloud_80 = true;
        }

        if (m_creature->GetHealthPercent() < 55.0f && !poisonous_cloud_55)
        {
            DoCastSpellIfCan(m_creature, SPELL_POISONOUS_CLOUD);
            poisonous_cloud_55 = true;
        }

        if (m_creature->GetHealthPercent() < 30.0f && !poisonous_cloud_30)
        {
            DoCastSpellIfCan(m_creature, SPELL_POISONOUS_CLOUD);
            poisonous_cloud_30 = true;
        }

        if (m_creature->GetHealthPercent() < 5.0f && !poisonous_cloud_5)
        {
            DoCastSpellIfCan(m_creature, SPELL_POISONOUS_CLOUD);
            poisonous_cloud_5 = true;
        }

        //ROT
        if (!m_creature->HasAura(SPELL_ROT))
            DoCastSpellIfCan(m_creature, SPELL_ROT);

        //DISMEMBER
        if (DISMEMBER_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_NEAREST, 0, nullptr, SELECT_FLAG_PLAYER), SPELL_DISMEMBER);
            DISMEMBER_TIMER = urand(25000,35000);
            DoScriptText(SAY_AGGRO_PUDGE, m_creature);
        }
        else DISMEMBER_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Boss_Pudge(Creature* pCreature)
{
    return new Boss_Pudge(pCreature);
}

//boss_gehrman
struct Boss_Gehrman : public ScriptedAI
{
    Boss_Gehrman(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 ANTIGUN_GEHRMAN_TIMER;
    bool blood_moon_90;
    bool blood_moon_65;
    bool vision_40;

    void Reset() override
    {
        ANTIGUN_GEHRMAN_TIMER = 1000;
        blood_moon_90 = false;
        blood_moon_65 = false;
        vision_40 = false;
        if (m_creature->HasAura(SPELL_BLOODBORNE))
            m_creature->RemoveAurasDueToSpell(SPELL_BLOODBORNE);
        if (m_creature->HasAura(SPELL_VISION))
            m_creature->RemoveAurasDueToSpell(SPELL_VISION);
    }

    void JustDied(Unit* Killer) override
    {
        DoScriptText(SAY_DEATH_THE_FIRST_HUNTER, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        DoCastSpellIfCan(m_creature, SPELL_BLOODBORNE);
        m_creature->CallForHelp(VISIBLE_RANGE);
        DoScriptText(SAY_AGGRO_THE_FIRST_HUNTER, m_creature);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //BLOOD_MOON && VISION
        if (m_creature->GetHealthPercent() < 90.0f && !blood_moon_90)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLOOD_MOON);
            blood_moon_90 = true;
        }

        if (m_creature->GetHealthPercent() < 65.0f && !blood_moon_65)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLOOD_MOON);
            blood_moon_65 = true;
        }

        if (m_creature->GetHealthPercent() < 40.0f && !vision_40)
        {
            DoCastSpellIfCan(m_creature, SPELL_VISION);
            vision_40 = true;
        }

        //BLOODBORNE
        if (!m_creature->HasAura(SPELL_BLOODBORNE))
            DoCastSpellIfCan(m_creature, SPELL_BLOODBORNE);

        //ANTIGUN_GEHRMAN
        if (!blood_moon_90 || blood_moon_65)
        {
            if (ANTIGUN_GEHRMAN_TIMER < uiDiff)
            {
                if (Unit* pTarget = m_creature->GetVictim())
                {
                    float antigun_gehrman_distance = m_creature->GetDistance(pTarget);
                    if (antigun_gehrman_distance >= 8.0f && antigun_gehrman_distance <= 40.0f)
                    {
                        DoCastSpellIfCan(pTarget, SPELL_ANTIGUN_GEHRMAN);
                        ANTIGUN_GEHRMAN_TIMER = urand(5500,6500);
                    }
                }
            }
            else ANTIGUN_GEHRMAN_TIMER -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Boss_Gehrman(Creature* pCreature)
{
    return new Boss_Gehrman(pCreature);
}

void AddSC_yharnam()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_545_transfur_patient";
    newscript->GetAI = &GetAI_Npc_TransfurPatientAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_545_bloodwolf";
    newscript->GetAI = &GetAI_Npc_BloodwolfAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_545_hemotherapy_patient";
    newscript->GetAI = &GetAI_Npc_HemotherapyPatientAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_545_yharnam_citizen";
    newscript->GetAI = &GetAI_Npc_YharnamCitizenAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_545_yharnam_guard";
    newscript->GetAI = &GetAI_Npc_YharnamGuardAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_545_yharnam_hunter";
    newscript->GetAI = &GetAI_Npc_YharnamHunterAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_545_yharnam_medic";
    newscript->GetAI = &GetAI_Npc_YharnamMedicAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_blood_starved_beast";
    newscript->GetAI = &GetAI_Boss_BloodStarvedBeast;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_father_gascoigne";
    newscript->GetAI = &GetAI_Boss_FatherGascoigne;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ludwig";
    newscript->GetAI = &GetAI_Boss_Ludwig;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_pudge";
    newscript->GetAI = &GetAI_Boss_Pudge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_gehrman";
    newscript->GetAI = &GetAI_Boss_Gehrman;
    newscript->RegisterSelf();
}
