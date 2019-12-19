// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
//
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"
#include "net.h"


/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints = boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1543593600, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    500        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1543593601,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1543593602,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x90;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0xfd;
        pchMessageStart[3] = 0xe9;
        vAlertPubKey = ParseHex("047c8d3eaf61275a344f2272d6c1dbe036906b917f39920ea223bbcc76664b56fd8a5e21fe62c7b906e1c60e2cf697025da3c58d013a5e55394860d81799e2a0d5");
        nDefaultPort = 45022;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // TICO starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 60 * 1; // TICO: 1 day
        nTargetSpacing = 60 * 1;  // TICO: 1 minute
        nMaturity = 1;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 28000000 * COIN;
        nMasternodeHoldCoins = 5000 * COIN;
        
        /** Height or Time Based Activations **/
        nLastPOWBlock = 61;
        nModifierUpdateBlock = 615800;
        nZerocoinStartHeight = 61;
        nZerocoinStartTime = 1543593600; // October 17, 2017 4:30:00 AM
        nBlockEnforceSerialRange = 895400; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "11/11/2018 - HOTEL BOOKING IN YOUR HANDS - Ticoin";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04ed7f57be5be9fd21f8fbdaee11104c46318bd2e12c24cd722adf4c8720c0799db54903c24d46491b3b83a4fb41836db81cbd506c663e30c32d632ef2ec1fedd8") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nTime = 1543593600;
        genesis.nBits = 0x1e0ffff0;
        
        genesis.nVersion = 1;
        genesis.nNonce = 2994456;
        hashGenesisBlock = genesis.GetHash();
        
        assert(hashGenesisBlock == uint256("0x000002c9aa0bbe5fa38167272c28572d8fe503865b7307b24a9aab51a1c05fd2"));
        assert(genesis.hashMerkleRoot == uint256("0x4ac304201084cd922a9a5554053f1c0856bfedc75623114ef39b8cf8ecb65391"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();
        
        vFixedSeeds.clear();
        vSeeds.clear();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
        
        vSeeds.push_back(CDNSSeedData("seeder", "seeder.ticoin.cc"));
        
        
        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 20;
        strSporkKey = "04c3d3bd7dc03b841e303d7338fa7d138e8983b0dde69c8965e9a6a54093f1be28b42f0941786a5da411993b0488e40b6f9b091e203911c4ce5fbbd679fc632586";
        strObfuscationPoolDummyAddress = "D12q1119j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1543593600; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 1; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 2;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;/**/

// /**
//  * Testnet (v3)
//  */
// class CTestNetParams : public CMainParams
// {
// public:
//     CTestNetParams()
//     {
//         networkID = CBaseChainParams::TESTNET;
//         strNetworkID = "test";
//         pchMessageStart[0] = 0x45;
//         pchMessageStart[1] = 0x76;
//         pchMessageStart[2] = 0x65;
//         pchMessageStart[3] = 0xba;
//         vAlertPubKey = ParseHex("000010e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9");
//         nDefaultPort = 39034;
//         nEnforceBlockUpgradeMajority = 51;
//         nRejectBlockOutdatedMajority = 75;
//         nToCheckBlockUpgradeMajority = 100;
//         nMinerThreads = 0;
//         nTargetTimespan = 1 * 60; // TICO: 1 day
//         nTargetSpacing = 1 * 60;  // TICO: 1 minute
//         nLastPOWBlock = 200;
//         nMaturity = 15;
//         nMasternodeCountDrift = 4;
//         nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
//         nMaxMoneyOut = 43199500 * COIN;
//         nZerocoinStartHeight = 201576;
//         nZerocoinStartTime = 1501776000;
//         nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
//         nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
//         nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
//         nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
//         nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
// 
//         //! Modify the testnet genesis block so the timestamp is valid for a later start.
//         genesis.nTime = 1454124731;
//         genesis.nNonce = 2402015;
// 
//         hashGenesisBlock = genesis.GetHash();
//         assert(hashGenesisBlock == uint256("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
// 
//         vFixedSeeds.clear();
//         vSeeds.clear();
//         vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "ticoin-testnet.seed.fuzzbawls.pw"));
//         vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "ticoin-testnet.seed2.fuzzbawls.pw"));
//         vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net"));
//         vSeeds.push_back(CDNSSeedData("88.198.192.110", "88.198.192.110"));
// 
//         base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet ticoin addresses start with 'x' or 'y'
//         base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet ticoin script addresses start with '8' or '9'
//         base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
//         // Testnet ticoin BIP32 pubkeys start with 'DRKV'
//         base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
//         // Testnet ticoin BIP32 prvkeys start with 'DRKP'
//         base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
//         // Testnet ticoin BIP44 coin type is '1' (All coin's testnet default)
//         base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
// 
//         convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
// 
//         fRequireRPCPassword = true;
//         fMiningRequiresPeers = true;
//         fAllowMinDifficultyBlocks = true;
//         fDefaultConsistencyChecks = false;
//         fRequireStandard = false;
//         fMineBlocksOnDemand = false;
//         fTestnetToBeDeprecatedFieldRPC = true;
// 
//         nPoolMaxTransactions = 2;
//         strSporkKey = "04348C2F50F90267E64FACC65BFDC9D0EB147D090872FB97ABAE92E9A36E6CA60983E28E741F8E7277B11A7479B626AC115BA31463AC48178A5075C5A9319D4A38";
//         strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
//         nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
//         nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
//                                        // here because we only have a 8 block finalization window on testnet
//     }
//     const Checkpoints::CCheckpointData& Checkpoints() const
//     {
//         return dataTestnet;
//     }
// };
// // static CTestNetParams testNetParams;
// 
// /**
//  * Regression test
//  */
// class CRegTestParams : public CTestNetParams
// {
// public:
//     CRegTestParams()
//     {
//         networkID = CBaseChainParams::REGTEST;
//         strNetworkID = "regtest";
//         strNetworkID = "regtest";
//         pchMessageStart[0] = 0xa1;
//         pchMessageStart[1] = 0xcf;
//         pchMessageStart[2] = 0x7e;
//         pchMessageStart[3] = 0xac;
//         nSubsidyHalvingInterval = 150;
//         nEnforceBlockUpgradeMajority = 750;
//         nRejectBlockOutdatedMajority = 950;
//         nToCheckBlockUpgradeMajority = 1000;
//         nMinerThreads = 1;
//         nTargetTimespan = 24 * 60 * 60; // Ticoin: 1 day
//         nTargetSpacing = 1 * 60;        // Ticoin: 1 minutes
//         bnProofOfWorkLimit = ~uint256(0) >> 1;
//         genesis.nTime = 1454124731;
//         genesis.nBits = 0x207fffff;
//         genesis.nNonce = 12345;
// 
//         hashGenesisBlock = genesis.GetHash();
//         nDefaultPort = 39036;
//         assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));
// 
//         vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
//         vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
// 
//         fRequireRPCPassword = false;
//         fMiningRequiresPeers = false;
//         fAllowMinDifficultyBlocks = true;
//         fDefaultConsistencyChecks = true;
//         fRequireStandard = false;
//         fMineBlocksOnDemand = true;
//         fTestnetToBeDeprecatedFieldRPC = false;
//     }
//     const Checkpoints::CCheckpointData& Checkpoints() const
//     {
//         return dataRegtest;
//     }
// };
// // static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 39038;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
//     case CBaseChainParams::TESTNET:
//         return testNetParams;
//     case CBaseChainParams::REGTEST:
//         return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}