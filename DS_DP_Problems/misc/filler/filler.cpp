#include <iostream>
#include <string>

int readFromConfig(const std::string fileToRead)

{

    std::ifstream fp;

    std::string LineBuffer;

    struct stat sbuf;

    const std::string WHITESPACE = " \t";

    if (stat(fileToRead.c_str(), &sbuf) != 0)

    {

        prompt("%s file does not exist\n",fileToRead.c_str());

        return -1;

    }

    fp.open( fileToRead.c_str(), std::fstream::in );

    if ( ! (fp.is_open()) )

    {

        prompt("%s Unable to open file\n",fileToRead.c_str());

        return -1;

    }

    std::string keyFromFile;

    std::string valueFromFile;

    short int lineNo = 0;

    std::size_t colon_pos = -1;

    while ( ! fp.eof() )

    {

        getline(fp, LineBuffer);

        lineNo++;

        if (LineBuffer.find_first_not_of(WHITESPACE) != std::string::npos)

            LineBuffer = LineBuffer.substr(LineBuffer.find_first_not_of(WHITESPACE));

        if (LineBuffer == "" || LineBuffer.at(0) == '#')

            continue;

        colon_pos = LineBuffer.find(':');

        if (colon_pos == std::string::npos)

            return lineNo;

        keyFromFile = LineBuffer.substr(0, colon_pos);

        valueFromFile = LineBuffer.substr(colon_pos+1);

        if (valueFromFile.find_first_not_of(WHITESPACE) != std::string::npos)

            valueFromFile = valueFromFile.substr(valueFromFile.find_first_not_of(WHITESPACE));

        if (valueFromFile.find_last_not_of(WHITESPACE) != std::string::npos)

            valueFromFile = valueFromFile.substr(0, (valueFromFile.find_last_not_of(WHITESPACE)) + 1);

        silentSetupMap[keyFromFile] = valueFromFile;

    }

    fp.close();

    return 0;

}

 

 

 

                // this should be a two-D map just like parameters - future

                typedef std::vector<keywords_t> keywords_list_t;

                keywords_list_t _keywords;

 

 

 

   if (stat(fileToRead.c_str(), &sbuf) != 0)

    {

        prompt("%s file does not exist\n",fileToRead.c_str());

        return 0;

    }

 

 

 

int get_RSA_public_key(const std::string &fileToRead, std::string *Key)

{

    std::ifstream fp;

    std::string LineBuffer;

    struct stat sbuf;

    *Key = "";

    if (stat(fileToRead.c_str(), &sbuf) != 0)

    {

        prompt("%s file does not exist\n",fileToRead.c_str());

        return 0;

    }

    fp.open( fileToRead.c_str(), std::fstream::in );

    if ( ! (fp.is_open()) )

    {

        prompt("%s Unable to open file\n",fileToRead.c_str());

        return 0;

    }

    while ( ! fp.eof() )

    {

        getline(fp, LineBuffer);

        if (LineBuffer.find("BEGIN PUBLIC KEY") != std::string::npos ||

            LineBuffer.find("END PUBLIC KEY") != std::string::npos )

                continue;

        *Key= *Key + LineBuffer;

    }

    fp.close();

    return 1;

}

 

 

/*  vim: sw=4:

*  Copyright (c) 2012, Quest, http://www.quest.com

* 

 *  ALL RIGHTS RESERVED.

* 

 *  This software is the confidential and proprietary information of

*  Quest Software Inc. ("Confidential Information"). You shall not

*  disclose such Confidential Information and shall use it only in

*  accordance with the terms of the license agreement you entered

*  into with Quest Software Inc.

* 

 *  Quest Software Inc. MAKES NO REPRESENTATIONS OR

*  WARRANTIES ABOUT THE SUITABILITY OF THE SOFTWARE,

*  EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED

*  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS

*  FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT. Quest

*  SHALL NOT BE LIABLE FOR ANY DAMAGES

*  SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING

*  OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

* 

 *  Author: Mike Nguyen

*/

 

 

#include <sp_sys.h>

#include <shareplex.h>

#include <util.h>

#include <slg.h>

 

#include <xpst_timer.h>

#include "pre_query.h"

#include "Serializer.h"

 

#include <sk/util/Class.h>

#include <sk/util/String.h>

 

namespace {

    const sk::util::String __className("Serializer");

}

 

Serializer::

Serializer(sk::util::String datasource)

{

    _que        = -1;

    _ddl_cnt    = 0;

    _datasource = datasource;

    PreSession::init();

    PreQuery::init(datasource);

    _child      = false;

    _shutdown_immediate = true;

    _name       = "Serializer";

    setLayer( SERIALIZER_LAYER );

}

 

Serializer::

~Serializer()

{

    funcLog("Serializer::~Serializer()\n" );

    for( TransMap::iterator iter = _trans.begin(); iter != _trans.end(); iter++)

    {

        delete iter->second;

    }

}

 

const sk::util::Class

Serializer::

getClass() const

{

    return sk::util::Class(__className);

}

 

void

Serializer::checkData( XpstOdr* o )

{

    funcLog("Serializer::checkData()\n" );

    if ( o->hasData() == false )

    {

        debugLog( LOG_LEVEL_3, "Serializer::checkData que=%d seq=" _I64" \n", _que, o->getQueSeq() );

        rim_msg_t rmsg;

        if ( queReread( o->getQueNum(), o->getQueSeq(), rmsg ) == false )

        {

            errorLog("queReread() failed\n" );

        }

        o->copyRimData( rmsg );

    }

}

 

void

Serializer::processOdr( XpstOdr* o, int pos )

{

    ProcTimerElapsed(XPST_TIMER_SERIALIZE);

    funcLog( "Serializer::processOdr(%d " _I64 ")\n", o->getQueNum(),

            o->getQueSeq() );

 

    o->info();

    pushDownOdr( o, pos );

    ProcTimerSet(XPST_TIMER_SERIALIZE);

}

 

bool

Serializer::isValidOp( const rim_msg_t& rmsg )

{

    bool rc = false;

    odr_t* odr = (odr_t *)rmsg.md;

    switch( odr->odr_op )

    {

        case  ODR_INSERT1:

        case  ODR_INSERTL:

        case  ODR_INSERTN:

        case  ODR_DELETEN:

        case  ODR_UPDATE1:

        case  ODR_UPDATEL:

        case  ODR_DELETE1:

        case  ODR_COMMIT:

        case  ODR_DLOAD:

        case  ODR_INSERT_BATCH:

        case  ODR_UPDATE_BATCH:

        case  ODR_DELETE_BATCH:

        case  ODR_TRUNC:

        case  ODR_NOP:

        case  ODR_DDL_ALTER_TABLE:

        case ODR_LOB:

        case ODR_LOB_CACHE:

        case ODR_LOBINDEX:

        case ODR_IC_LONG:

        case ODR_IGNOREL:

        case ODR_FULL_ROLLBACK:

        case ODR_DDL_UPDATE_INTERNAL:

        case ODR_DDL_OCACHE:

            rc = true;

            break;

        // ignore the ops

        case ODR_NULL:

        case ODR_DONE:

        case ODR_LINKL:

        case ODR_LOB_INFO:

            break;

              case ODR_DDL_CREATE_TABLE:

              case ODR_DDL_DROP_TABLE:

        case ODR_DDL:

              case ODR_9I_DDL:

            queRelease( getQueNum( rmsg ), getQueSeq( rmsg ) );

            break;

        default:

            noticeLog( "Serializer invalid op(%d) %s\n",

                odr->odr_op, odr_op2str( odr->odr_op ));

            break;

    }

    return rc;

}

 

std::string

Serializer::getTransId( const rim_msg_t& rmsg )

{

    char trans[128];

    odr_t* odr = (odr_t *)rmsg.md;

    oracle_transid_tostring_r( &odr->odr_trans, trans );

    return trans;

}

 

Transaction*

Serializer::getTrans( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::getTrans()\n" );

    Transaction* t = findTrans( rmsg );

    _que = getQueNum(rmsg);

    if ( t == NULL )

    {

        t =  new Transaction( getLayer() );

        _trans[getTransId( rmsg )] = t;

    }

    return t;

}

 

void

Serializer::removeTrans( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::removeTrans()\n" );

    TransMap::iterator iter = _trans.find( getTransId(rmsg) );

    if ( iter != _trans.end() )

    {

        _trans.erase( iter );

    }

}

 

Transaction*

Serializer::findTrans( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::findTrans()\n" );

    TransMap::iterator iter = _trans.find( getTransId(rmsg) );

    if ( iter == _trans.end() )

    {

        return 0;

    }

    return iter->second;

}

 

void

Serializer::partialRollback( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::partialRollback)\n" );

 

    // ODR_PARTIAL_ROLLBACK is from out of bound message, ignore it

    // wait for in bound ODR_PARTIAL_ROLLBACK

    if ( getQueNum( rmsg ) == -2 )

    {

        return;

    }

 

    odr_t* odr = (odr_t *)rmsg.md;

    Transaction* t = findTrans( rmsg );

 

    // check for valid trans

   

    if ( t == 0 )

    {

        // From oob message, the data is not in yet, so just ignore

        return;

    }

    t->info();

 

    // If full rollback transaction, then ignore partial rollback messages

    if ( t->getFullRollback() == false )

    {

        rollback( *t, *odr );

    }

}

 

bool Serializer::queRelease( sp_int4_t que_num, que_seq_t seq )

{

    if(getDownStreamCount() > 1)

    {

        debugLog_4("PEP queRelease que %d seq " _I64 "\n", que_num, seq );

        if(_preSession.size() >= que_num)

            _preSession[que_num-1]->clean(seq);

    }

    return OdrStream::queRelease( que_num, seq );

}

 

int Serializer::getNextThread( Transaction* t, XpstOdr *o )

{

    if(getDownStreamCount() > 1 && o->getQueNum() > 0)

    {

        int i;

        XpstOdr *r;

        unsigned cnt = t->getOdrCnt();

        int block = 0;

        int batch_skip = 0;

        int thread = (o->getQueNum() - 1) % _num_threads;

        int session = o->getQueNum() - 1;

        int sync;

        int pos;

 

        debugLog_4("PEP getNextThread %d num_threads %d\n", thread, _num_threads );

        ProcTimerElapsedSet(XPST_TIMER_SERIALIZE, XPST_TIMER_CHECK_DEP);

        for(pos = _preSession.size(); pos <= session; pos++)

        {

            _preSession.push_back(new PreSession(_preSession.size()+1,7019));

        }

 

        for ( i = 0; i < cnt; i++ )

        {

            r = t->getOdr( i );

            if ( r->hasData() == false )

            {

                block = -1;

                _preSession[session]->wipe();

                break;

            }

            thread = (r->getQueNum() - 1) % _num_threads;

            debugLog_4("PEP getNextThread i %d thread %d num_threads %d\n",i , thread, _num_threads );

            sync = _preSession[session]->check(r->getCdr(),batch_skip,true);

            debugLog_4("PEP preSession B  op %s seq/offset %d/" _U64 " table %s check return %d\n",

                   odr_op2str(r->getOp()), r->getRedoSeq(), r->getRedoOffset(),

                   r->getObjectNameNoQ().c_str(), sync );

            if(r->getOp() == ODR_COMMIT)

                continue;

            if(sync < 0)

                block = sync;

            else if(sync > 0)

            {

                if(block == 0)

                    block = sync;

                else if(block != sync)

                    block = -1;

            }

        }

        sync = _preSession[session]->check(o->getCdr(),batch_skip,true);

        debugLog_4("PEP preSession A  op %s seq/offset %d/" _U64 " check return %d\n",

               odr_op2str(o->getOp()), o->getRedoSeq(), o->getRedoOffset(), sync );

        ProcTimerElapsedSet(XPST_TIMER_CHECK_DEP, XPST_TIMER_WAIT_DEP);

        if(sync < 0 && o->getOp() != ODR_COMMIT)

            block = sync;

        if(block < 0)

        {

            debugLog_4("PEP flushData to count %d\n", getDownStreamCount() );

            for(i=0;i<getDownStreamCount();i++)

                flushData(i);

            block = 0;

        }

        else if(block > 0)

        {

            int block_thread = (block-1) % _num_threads;

            if(block_thread != thread)

            {

                debugLog_4("PEP flushData thread %d\n", block_thread );

                flushData(block_thread);

            }

        }

 

        ProcTimerElapsedSet(XPST_TIMER_WAIT_DEP, XPST_TIMER_SERIALIZE);

        debugLog_4("PEP getNextThread return %d\n", thread );

        return thread % _num_threads;

    }

    else

        return 0;

}

 

void

Serializer::commitTrans( Transaction* t, const rim_msg_t& rmsg )

{

    unsigned cnt = t->getOdrCnt();

    bool start_trans = true;

 

    funcLog( "Serializer::commitTrans(%u)\n", cnt );

 

    odr_t* odr = (odr_t *)rmsg.md;

 

    XpstOdr *r;

    unsigned i;

    // push down all odr messages down the stream

    int tot_dml = t->getDmlCnt();

    XpstOdr* o = new XpstOdr( rmsg, getLayer() );

    int thread = getNextThread(t, o);

 

    for ( i = 0; i < cnt; i++ )

    {

        debugLog_4("commit record %d\n", i );

        if(!_child)

        {

            wait_for_udp( 0 );

 

            if ( ShutDown )

            {

                if ( _shutdown_immediate )

                {

                    debugLog_4("Serializer shutDown()\n" );

                    shutDown();

                    return;

                }

                else if (i % 1000 == 0)

                {

                    log_write("finishing transaction before shutdown. currently at %d of %d.\n", i, cnt);

                }

            }

        }

 

        if ( isStopOnError() )

        {

            debugLog_4("stop on error()\n" );

            return;

        }

 

        r = t->getOdr( i );

        checkData( r );

 

        if ( r->isNop() )

        {

            delete r;

            continue;

        }

 

        r->setCommitSCN( odr->odr_scn );

        r->setCommitTime( odr->odr_time );

        if ( start_trans == true )

        {

            r->setBeginTrans();

            start_trans = false;

        }

        r->setMsgTot( tot_dml );

 

        if ( r->isLobObject() )

        {

            t->processLob( *r, *(r->getOdr()) );

        }

        processOdr( r, thread );

    }

 

    // send commit down the stream

    o->setCommitTime( odr->odr_time );

    o->setTrans( t );

    processOdr( o, thread );

    removeTrans( rmsg );

 

    if ( ShutDown && !_shutdown_immediate )

    {

        debugLog_4("Serializer shutDown()\n" );

        shutDown();

    }

}

 

 

void

Serializer::processRim( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::processRim()\n" );

 

    odr_t* odr = (odr_t *)rmsg.md;

 

    if ( isDebugLevel_2() )

    {

 

        log_write("que=%d seq="_I64 " %s %u %u %d/" _U64 "\n",

            getQueNum( rmsg ),

            getQueSeq( rmsg ),

            odr_op2str(odr->odr_op),

            odr->odr_forback, odr->odr_opnum,

                  odr->odr_seqno, odr->odr_offset );

    }

 

    if ( XpstOdr::isControlMsg( odr->odr_op ) )

    {

        XpstOdr* o = new XpstOdr( rmsg, getLayer() );

        Transaction* t = getTrans( rmsg );

        processOdr( o, getNextThread(t, o) );

        return;

    }

 

    // Take care partial rollbacks first

    if ( XpstOdr::isPartialRollback( odr->odr_op ))

    {

        partialRollback( rmsg );

        return;

    }

 

    // not expect to see single rollback ops any more

    // we are using collapsing logic

    if ( XpstOdr::isRollback( odr->odr_flags ) == true &&

        odr->odr_op != ODR_COMMIT && odr->odr_op != ODR_FULL_ROLLBACK )

    {

        processBackwardOp( rmsg );

        return;

    }

 

    if ( needRollback( rmsg ) )

    {

        rollback( rmsg );

        // do not return here, the current op is + forward

        // and we need to process it

    }

 

    // Non or unsupport operations will be ignored

    if ( isValidOp( rmsg ) == false )

    {

        return;

    }

 

    if ( getQueNum( rmsg ) > 5000 &&

        ( odr->odr_op == ODR_LOB || odr->odr_op == ODR_LOB_CACHE ))

    {

        Transaction::addHiSubQueVarrayLob( rmsg );

        return;

    }

    Transaction* t = getTrans( rmsg );

 

    // Complete rollback

    if ( XpstOdr::isRollback( odr->odr_flags ) == true &&

        odr->odr_op == ODR_COMMIT )

    {

        debugLog_4("Send Complete Rollback down the stream\n" );

        XpstOdr* o = new XpstOdr( rmsg, getLayer() );

        processOdr( o, getNextThread(t, o) );

        t->completeRollback();

        removeTrans( rmsg );

        delete t;

        return;

    }

 

    // Commit transaction, push down the messages

    switch ( odr->odr_op  )

    {

        case ODR_COMMIT:

            commitTrans( t, rmsg );

            break;

        case ODR_TRUNC:

        case ODR_DDL_ALTER_TABLE:

        case ODR_DDL_UPDATE_INTERNAL:

        {

            XpstOdr* o = new XpstOdr( rmsg, getLayer() );

            int thread = getNextThread(t, o);

            processOdr( o, thread );

            removeTrans( rmsg );

            delete t;

            flushData(thread);

            break;

        }

        case ODR_LOB:

        case ODR_LOB_CACHE:

            t->addLob( rmsg );

            break;

        case ODR_IC_LONG:

            t->addLong( rmsg );

            break;

        case ODR_IGNOREL:

            t->removeLong();

            break;

        case ODR_FULL_ROLLBACK:

            t->setFullRollback();

            break;

        default:

            if ( t->getFullRollback() == false )

            {

                t->add( rmsg  );

            }

            break;

    }

}

 

void

Serializer::logOdr( odr_t& odr )

{

    odr_compute_only( &odr, 1);

    odr_print( &odr );

}

 

void

Serializer::rollback( Transaction& t, const odr_t& odr )

{

    funcLog("Serializer::rollback(%d,%d)\n", odr.odr_forback, odr.odr_opnum );

    if (  odr.odr_forback == 0 )

    {

        t.completeRollback();

        return;

    }

 

    XpstOdr* o;

    sp_int4_t forback;

    bool found = false;

 

    for( o = t.getLastOdr(); o != NULL; o = t.getLastOdr() )

    {

        forback = o->getForBack();

        if ( forback > odr.odr_forback )

        {

            t.removeLastOdr();

        }

        else if ( forback == odr.odr_forback )

        {

            t.removeLastOdr();

            found = true;

            break;

        }

        else

        {

            odr_t* rec = o->getOdr();

            if ( rec == NULL )

            {

                rim_msg_t rmsg;

                if ( queReread( o->getQueNum(), o->getQueSeq(), rmsg) == false )

                {

                    errorLog("queReread() failed\n" );

                }

                rec = (odr_t *)rmsg.md;

                odr_compute_only( rec, 1 );

            }

            // save old nrow

            int nrow = o->getExecuteRows();

            found = o->isRollbackBatch( *rec, odr.odr_forback );

 

            // Recalculate the number of real DMLs

            if ( found == true )

            {

                t.decDmlCnt( nrow );

                t.incDmlCnt( o->getExecuteRows() );

            }

            break;

        }

    }

 

    if ( found == false )

    {

        t.dump();

        errorInternalLog("can not find the start of forback %d\n",

            odr.odr_forback );

    }

    t.info();

    funcLog("Serializer::rollback(%d) done\n", odr.odr_forback );

}

 

unsigned

Serializer::_read_release_size = 100;

 

 

void

Serializer::

setReadReleaseSize( unsigned sz )

{

    _read_release_size = sz;

}

 

int Serializer::_num_threads = 1;

 

void Serializer:: setThreads( int threads )

{

    _num_threads = threads;

}

 

void

Serializer::switchFile(int pos)

{

    OdrStream::switchFile(pos);

}

 

void

Serializer::processBackwardOp( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::%s\n", __FUNCTION__ );

    odr_t* odr = (odr_t *)rmsg.md;

    Transaction* t = findTrans( rmsg );

 

    // check for valid trans

    if ( t == 0 )

    {

        return;

    }

    t->setPartialRollback( odr->odr_forward );

}

 

void

Serializer::rollback( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::%s\n", __FUNCTION__ );

    Transaction* t = findTrans( rmsg );

 

    // check for valid trans

    if ( t == 0 )

    {

        return;

    }

 

    sp_int4_t backward_cnt = t->getBackwardCnt();

 

    debugLog_5("backward_cnt=%d\n", backward_cnt );

 

    if ( backward_cnt >= 0 )

    {

        t->clearPartialRollback();

        return;

    }

    sp_int4_t cnt = 0;

 

    for( XpstOdr* o = t->getLastOdr(); o != NULL; o = t->getLastOdr() )

    {

        cnt = backward_cnt + o->getForwardCnt();

        debugLog_5( "process %s/%s cnt = %d\n", o->getQueNumStr().c_str(),

            o->getQueSeqStr().c_str(), cnt );

        if ( cnt < 0 )

        {

            t->removeLastOdr();

        }

        // batch op

        else if ( cnt > 0 )

        {

            rollbackBatch( *t, *o, cnt );

            break;

        }

        else

        {

            t->removeLastOdr();

            break;

        }

        backward_cnt = cnt;

    }

    t->clearPartialRollback();

}

 

bool

Serializer::needRollback( const rim_msg_t& rmsg )

{

    funcLog( "Serializer::%s\n", __FUNCTION__ );

    odr_t* odr = (odr_t *)rmsg.md;

    Transaction* t = findTrans( rmsg );

 

    // check for valid trans

    if ( t == 0 )

    {

        return false;

    }

 

    if ( odr->odr_forward >= 0 && t->isPartialRollback() == true &&

        t->getBackwardCnt() < 0 )

    {

        return true;

    }

    return false;

}

 

void

Serializer::rollbackBatch( Transaction& t, XpstOdr& o, int cnt )

{

    funcLog( "Serializer::%s %d\n", __FUNCTION__, cnt );

    odr_t* rec = o.getOdr();

    if ( rec == NULL )

    {

        rim_msg_t rmsg;

        if ( queReread( o.getQueNum(), o.getQueSeq(), rmsg) == false )

        {

            errorLog("queReread() failed\n" );

        }

        rec = (odr_t *)rmsg.md;

        odr_compute_only( rec, 1 );

    }

    int nrow = o.getExecuteRows();

    if ( o.rollbackBatch( *rec, cnt ) )

    {

        t.decDmlCnt( nrow );

        t.incDmlCnt( o.getExecuteRows() );

    }

}

 

void

Serializer::setShutdownImmediate(bool flag)

{

    _shutdown_immediate = flag;

}

 

bool

Serializer::getShutdownImmediate()

{

    return _shutdown_immediate;

}
