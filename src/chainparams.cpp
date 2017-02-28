// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>
#include <stdint.h>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
  0x01020304,
};

static const unsigned int timeMainGenesisBlock = 1487177588;
uint256 hashMainGenesisBlock("0x00000090d4dcdef966b728f50fb8fae857bf77057559d8ab559e848ecde6a71b");
static CBigNum bnMainProofOfWorkLimit(~uint256(0) >> 20);

static const int64_t nGenesisBlockRewardCoin = 1 * COIN;

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfe;
        pchMessageStart[1] = 0xa5;
        pchMessageStart[2] = 0x03;
        pchMessageStart[3] = 0xdd;
        vAlertPubKey = ParseHex("0493e6dc310a0e444cfb20f3234a238f77699806d47909a42481010c5ce68ff04d3babc959cd037bd3aa6ded929f2b9b4aa2f626786cd7f8495e5bb61e9cfebbc4");
        nDefaultPort = 55669;
        nRPCPort = 8644;
        bnProofOfWorkLimit = bnMainProofOfWorkLimit;
        nSubsidyHalvingInterval = 60480;

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        const char* pszTimestamp = "15 February 2017, CoinDesk, Microsoft Teams With KPMG to Launch Blockchain Workspace Network";

        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = nGenesisBlockRewardCoin;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        //printf("scriptSig: %s\n", txNew.vin[0].scriptSig.ToString().c_str());
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 112;
        genesis.nTime    = timeMainGenesisBlock;
        genesis.nBits    = bnMainProofOfWorkLimit.GetCompact();//0x1e0ffff0
        genesis.nNonce   = 908750953;

        /*
        CDataStream ssBlock(SER_NETWORK, PROTOCOL_VERSION);
        ssBlock << genesis;
        printf("block: %s\n", HexStr(ssBlock.begin(), ssBlock.end()).c_str());

        printf("block size: %i\n", (int)::GetSerializeSize(genesis, SER_NETWORK, PROTOCOL_VERSION));
        printf("tx size: %i\n", (int)::GetSerializeSize(genesis.vtx[0], SER_NETWORK, PROTOCOL_VERSION));
        */

        /*
        hashGenesisBlock = genesis.GetHash();
        while (hashGenesisBlock > bnProofOfWorkLimit.getuint256()){
            if (++genesis.nNonce==0) break;
            hashGenesisBlock = genesis.GetHash();
        }
        printf("hashGenesisBlock: %s\n", hashGenesisBlock.ToString().c_str());
        printf("hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("CBlock(hash=%s, ver=%d, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
            genesis.GetHash().ToString().c_str(),
            genesis.nVersion,
            genesis.hashPrevBlock.ToString().c_str(),
            genesis.hashMerkleRoot.ToString().c_str(),
            genesis.nTime, genesis.nBits, genesis.nNonce,
            genesis.vtx.size());
        */

        hashGenesisBlock = genesis.GetHash();
        assert(genesis.hashMerkleRoot == uint256("0xbba157b0ac1d10baa1dbb3f11c4bb03f27d5953d26f1bc05bdf1081ac9eab3f5"));
        assert(hashGenesisBlock == hashMainGenesisBlock);

        vSeeds.push_back(CDNSSeedData("s1.bitrevshares.com", "s1.bitrevshares.com"));
        vSeeds.push_back(CDNSSeedData("s2.bitrevshares.com", "s2.bitrevshares.com"));
        vSeeds.push_back(CDNSSeedData("seed1.bitrevshares.com", "seed1.bitrevshares.com"));
        vSeeds.push_back(CDNSSeedData("seed2.bitrevshares.com", "seed2.bitrevshares.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(60);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(9);
        base58Prefixes[SECRET_KEY] =     list_of(186);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0x1a;
        pchMessageStart[2] = 0x39;
        pchMessageStart[3] = 0xf7;
        vAlertPubKey = ParseHex("04218bc3f08237baa077cb1b0e5a81695fcf3f5b4e220b4ad274d05a31d762dd4e191efa7b736a24a32d6fd9ac1b5ebb2787c70e9dfad0016a8b32f7bd2520dbd5");
        nDefaultPort = 65669;
        nRPCPort = 18644;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1487177600;
        genesis.nNonce = 911043093;
        hashGenesisBlock = genesis.GetHash();

        hashGenesisBlock = genesis.GetHash();
        while (hashGenesisBlock > bnProofOfWorkLimit.getuint256()){
            if (++genesis.nNonce==0) break;
            hashGenesisBlock = genesis.GetHash();
        }

        assert(hashGenesisBlock == uint256("0x000002251f05cc61714724d1d971d808ba0f4c07dc11e8d5987946dcf70e82de"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(132);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(199);
        base58Prefixes[SECRET_KEY]     = list_of(247);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;

//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1487177700;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 8;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 65669;
        strDataDir = "regtest";

        assert(hashGenesisBlock == uint256("0x9c2f8327491798dbc656f6353e91df0669ba4d73ee1362cac57cad190c8637d6"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
