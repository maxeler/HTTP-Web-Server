/**\file */
#ifndef SLIC_DECLARATIONS_httpServer_2014_2_H
#define SLIC_DECLARATIONS_httpServer_2014_2_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define httpServer_2014_2_maxNumberOfSockets (16)
#define httpServer_2014_2_USE_NETWORK_MANAGER (1)


/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] ticks_KernelLMem_Read_CommandAndDataStream The number of ticks for which kernel "KernelLMem_Read_CommandAndDataStream" will run.
 * \param [in] ticks_KernelLMem_Write_CommandAndDataStream The number of ticks for which kernel "KernelLMem_Write_CommandAndDataStream" will run.
 * \param [in] inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts Input scalar parameter "KernelLMem_Read_CommandAndDataStream.totalBursts".
 * \param [in] inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst Input scalar parameter "KernelLMem_Read_CommandAndDataStream.wordsPerBurst".
 * \param [in] inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts Input scalar parameter "KernelLMem_Write_CommandAndDataStream.totalBursts".
 * \param [in] inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst Input scalar parameter "KernelLMem_Write_CommandAndDataStream.wordsPerBurst".
 * \param [out] outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_eofCount Output scalar parameter "SlicerFramer_network_mgmt_stream_from_host_tx_framed.eofCount".
 * \param [out] outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_sofCount Output scalar parameter "SlicerFramer_network_mgmt_stream_from_host_tx_framed.sofCount".
 * \param [out] outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails Output scalar parameter "TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numParityFails".
 * \param [out] outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads Output scalar parameter "TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numReads".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdRdCount Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdRdCount".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrBytes Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdWrBytes".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrCount Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdWrCount".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_rspCount Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.rspCount".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numParityFails".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numReads".
 * \param [out] outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_eofCount Output scalar parameter "network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor.eofCount".
 * \param [out] outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_sofCount Output scalar parameter "network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor.sofCount".
 * \param [in] instream_fromCpu Stream "fromCpu".
 * \param [in] instream_size_fromCpu The size of the stream instream_fromCpu in bytes.
 * \param [in] instream_network_mgmt_stream_from_host_tx_framed Stream "network_mgmt_stream_from_host_tx_framed".
 * \param [in] instream_size_network_mgmt_stream_from_host_tx_framed The size of the stream instream_network_mgmt_stream_from_host_tx_framed in bytes.
 * \param [out] outstream_network_mgmt_stream_to_host_rx_framed Stream "network_mgmt_stream_to_host_rx_framed".
 * \param [in] outstream_size_network_mgmt_stream_to_host_rx_framed The size of the stream outstream_network_mgmt_stream_to_host_rx_framed in bytes.
 * \param [out] outstream_rxPopDebug Stream "rxPopDebug".
 * \param [in] outstream_size_rxPopDebug The size of the stream outstream_rxPopDebug in bytes.
 * \param [out] outstream_toCpu Stream "toCpu".
 * \param [in] outstream_size_toCpu The size of the stream outstream_toCpu in bytes.
 * \param [out] outstream_toCpuFileSizeBytes Stream "toCpuFileSizeBytes".
 * \param [in] outstream_size_toCpuFileSizeBytes The size of the stream outstream_toCpuFileSizeBytes in bytes.
 * \param [out] outstream_toCpuSocketNumber Stream "toCpuSocketNumber".
 * \param [in] outstream_size_toCpuSocketNumber The size of the stream outstream_toCpuSocketNumber in bytes.
 * \param [in] inmem_CrcIndexTable_romCrcIndex1 Mapped ROM inmem_CrcIndexTable_romCrcIndex1, should be of size (32768 * sizeof(uint64_t)).
 * \param [in] inmem_CrcIndexTable_romCrcIndex2 Mapped ROM inmem_CrcIndexTable_romCrcIndex2, should be of size (32768 * sizeof(uint64_t)).
 */
void httpServer_2014_2(
	uint64_t ticks_KernelLMem_Read_CommandAndDataStream,
	uint64_t ticks_KernelLMem_Write_CommandAndDataStream,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst,
	uint64_t *outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_eofCount,
	uint64_t *outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_sofCount,
	uint64_t *outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails,
	uint64_t *outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdRdCount,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrBytes,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrCount,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_rspCount,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads,
	uint64_t *outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_eofCount,
	uint64_t *outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_sofCount,
	const void *instream_fromCpu,
	size_t instream_size_fromCpu,
	const void *instream_network_mgmt_stream_from_host_tx_framed,
	size_t instream_size_network_mgmt_stream_from_host_tx_framed,
	void *outstream_network_mgmt_stream_to_host_rx_framed,
	size_t outstream_size_network_mgmt_stream_to_host_rx_framed,
	void *outstream_rxPopDebug,
	size_t outstream_size_rxPopDebug,
	void *outstream_toCpu,
	size_t outstream_size_toCpu,
	void *outstream_toCpuFileSizeBytes,
	size_t outstream_size_toCpuFileSizeBytes,
	void *outstream_toCpuSocketNumber,
	size_t outstream_size_toCpuSocketNumber,
	const uint64_t *inmem_CrcIndexTable_romCrcIndex1,
	const uint64_t *inmem_CrcIndexTable_romCrcIndex2);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] ticks_KernelLMem_Read_CommandAndDataStream The number of ticks for which kernel "KernelLMem_Read_CommandAndDataStream" will run.
 * \param [in] ticks_KernelLMem_Write_CommandAndDataStream The number of ticks for which kernel "KernelLMem_Write_CommandAndDataStream" will run.
 * \param [in] inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts Input scalar parameter "KernelLMem_Read_CommandAndDataStream.totalBursts".
 * \param [in] inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst Input scalar parameter "KernelLMem_Read_CommandAndDataStream.wordsPerBurst".
 * \param [in] inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts Input scalar parameter "KernelLMem_Write_CommandAndDataStream.totalBursts".
 * \param [in] inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst Input scalar parameter "KernelLMem_Write_CommandAndDataStream.wordsPerBurst".
 * \param [out] outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_eofCount Output scalar parameter "SlicerFramer_network_mgmt_stream_from_host_tx_framed.eofCount".
 * \param [out] outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_sofCount Output scalar parameter "SlicerFramer_network_mgmt_stream_from_host_tx_framed.sofCount".
 * \param [out] outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails Output scalar parameter "TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numParityFails".
 * \param [out] outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads Output scalar parameter "TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numReads".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdRdCount Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdRdCount".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrBytes Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdWrBytes".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrCount Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdWrCount".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_rspCount Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.rspCount".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numParityFails".
 * \param [out] outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numReads".
 * \param [out] outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_eofCount Output scalar parameter "network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor.eofCount".
 * \param [out] outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_sofCount Output scalar parameter "network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor.sofCount".
 * \param [in] instream_fromCpu Stream "fromCpu".
 * \param [in] instream_size_fromCpu The size of the stream instream_fromCpu in bytes.
 * \param [in] instream_network_mgmt_stream_from_host_tx_framed Stream "network_mgmt_stream_from_host_tx_framed".
 * \param [in] instream_size_network_mgmt_stream_from_host_tx_framed The size of the stream instream_network_mgmt_stream_from_host_tx_framed in bytes.
 * \param [out] outstream_network_mgmt_stream_to_host_rx_framed Stream "network_mgmt_stream_to_host_rx_framed".
 * \param [in] outstream_size_network_mgmt_stream_to_host_rx_framed The size of the stream outstream_network_mgmt_stream_to_host_rx_framed in bytes.
 * \param [out] outstream_rxPopDebug Stream "rxPopDebug".
 * \param [in] outstream_size_rxPopDebug The size of the stream outstream_rxPopDebug in bytes.
 * \param [out] outstream_toCpu Stream "toCpu".
 * \param [in] outstream_size_toCpu The size of the stream outstream_toCpu in bytes.
 * \param [out] outstream_toCpuFileSizeBytes Stream "toCpuFileSizeBytes".
 * \param [in] outstream_size_toCpuFileSizeBytes The size of the stream outstream_toCpuFileSizeBytes in bytes.
 * \param [out] outstream_toCpuSocketNumber Stream "toCpuSocketNumber".
 * \param [in] outstream_size_toCpuSocketNumber The size of the stream outstream_toCpuSocketNumber in bytes.
 * \param [in] inmem_CrcIndexTable_romCrcIndex1 Mapped ROM inmem_CrcIndexTable_romCrcIndex1, should be of size (32768 * sizeof(uint64_t)).
 * \param [in] inmem_CrcIndexTable_romCrcIndex2 Mapped ROM inmem_CrcIndexTable_romCrcIndex2, should be of size (32768 * sizeof(uint64_t)).
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *httpServer_2014_2_nonblock(
	uint64_t ticks_KernelLMem_Read_CommandAndDataStream,
	uint64_t ticks_KernelLMem_Write_CommandAndDataStream,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst,
	uint64_t *outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_eofCount,
	uint64_t *outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_sofCount,
	uint64_t *outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails,
	uint64_t *outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdRdCount,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrBytes,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrCount,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_rspCount,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails,
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads,
	uint64_t *outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_eofCount,
	uint64_t *outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_sofCount,
	const void *instream_fromCpu,
	size_t instream_size_fromCpu,
	const void *instream_network_mgmt_stream_from_host_tx_framed,
	size_t instream_size_network_mgmt_stream_from_host_tx_framed,
	void *outstream_network_mgmt_stream_to_host_rx_framed,
	size_t outstream_size_network_mgmt_stream_to_host_rx_framed,
	void *outstream_rxPopDebug,
	size_t outstream_size_rxPopDebug,
	void *outstream_toCpu,
	size_t outstream_size_toCpu,
	void *outstream_toCpuFileSizeBytes,
	size_t outstream_size_toCpuFileSizeBytes,
	void *outstream_toCpuSocketNumber,
	size_t outstream_size_toCpuSocketNumber,
	const uint64_t *inmem_CrcIndexTable_romCrcIndex1,
	const uint64_t *inmem_CrcIndexTable_romCrcIndex2);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	uint64_t ticks_KernelLMem_Read_CommandAndDataStream; /**<  [in] The number of ticks for which kernel "KernelLMem_Read_CommandAndDataStream" will run. */
	uint64_t ticks_KernelLMem_Write_CommandAndDataStream; /**<  [in] The number of ticks for which kernel "KernelLMem_Write_CommandAndDataStream" will run. */
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts; /**<  [in] Input scalar parameter "KernelLMem_Read_CommandAndDataStream.totalBursts". */
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst; /**<  [in] Input scalar parameter "KernelLMem_Read_CommandAndDataStream.wordsPerBurst". */
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts; /**<  [in] Input scalar parameter "KernelLMem_Write_CommandAndDataStream.totalBursts". */
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst; /**<  [in] Input scalar parameter "KernelLMem_Write_CommandAndDataStream.wordsPerBurst". */
	uint64_t *outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_eofCount; /**<  [out] Output scalar parameter "SlicerFramer_network_mgmt_stream_from_host_tx_framed.eofCount". */
	uint64_t *outscalar_SlicerFramer_network_mgmt_stream_from_host_tx_framed_sofCount; /**<  [out] Output scalar parameter "SlicerFramer_network_mgmt_stream_from_host_tx_framed.sofCount". */
	uint64_t *outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails; /**<  [out] Output scalar parameter "TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numParityFails". */
	uint64_t *outscalar_TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads; /**<  [out] Output scalar parameter "TCPRxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numReads". */
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdRdCount; /**<  [out] Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdRdCount". */
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrBytes; /**<  [out] Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdWrBytes". */
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_cmdWrCount; /**<  [out] Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.cmdWrCount". */
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_rspCount; /**<  [out] Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1.rspCount". */
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numParityFails; /**<  [out] Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numParityFails". */
	uint64_t *outscalar_TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter_numReads; /**<  [out] Output scalar parameter "TCPTxWindowMemory_QSFP_BOT_10G_PORT1_QMemAdapter.numReads". */
	uint64_t *outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_eofCount; /**<  [out] Output scalar parameter "network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor.eofCount". */
	uint64_t *outscalar_network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor_sofCount; /**<  [out] Output scalar parameter "network_mgmt_stream_to_host_rx_framed_FramedStreamPreprocessor.sofCount". */
	const void *instream_fromCpu; /**<  [in] Stream "fromCpu". */
	size_t instream_size_fromCpu; /**<  [in] The size of the stream instream_fromCpu in bytes. */
	const void *instream_network_mgmt_stream_from_host_tx_framed; /**<  [in] Stream "network_mgmt_stream_from_host_tx_framed". */
	size_t instream_size_network_mgmt_stream_from_host_tx_framed; /**<  [in] The size of the stream instream_network_mgmt_stream_from_host_tx_framed in bytes. */
	void *outstream_network_mgmt_stream_to_host_rx_framed; /**<  [out] Stream "network_mgmt_stream_to_host_rx_framed". */
	size_t outstream_size_network_mgmt_stream_to_host_rx_framed; /**<  [in] The size of the stream outstream_network_mgmt_stream_to_host_rx_framed in bytes. */
	void *outstream_rxPopDebug; /**<  [out] Stream "rxPopDebug". */
	size_t outstream_size_rxPopDebug; /**<  [in] The size of the stream outstream_rxPopDebug in bytes. */
	void *outstream_toCpu; /**<  [out] Stream "toCpu". */
	size_t outstream_size_toCpu; /**<  [in] The size of the stream outstream_toCpu in bytes. */
	void *outstream_toCpuFileSizeBytes; /**<  [out] Stream "toCpuFileSizeBytes". */
	size_t outstream_size_toCpuFileSizeBytes; /**<  [in] The size of the stream outstream_toCpuFileSizeBytes in bytes. */
	void *outstream_toCpuSocketNumber; /**<  [out] Stream "toCpuSocketNumber". */
	size_t outstream_size_toCpuSocketNumber; /**<  [in] The size of the stream outstream_toCpuSocketNumber in bytes. */
	const uint64_t *inmem_CrcIndexTable_romCrcIndex1; /**<  [in] Mapped ROM inmem_CrcIndexTable_romCrcIndex1, should be of size (32768 * sizeof(uint64_t)). */
	const uint64_t *inmem_CrcIndexTable_romCrcIndex2; /**<  [in] Mapped ROM inmem_CrcIndexTable_romCrcIndex2, should be of size (32768 * sizeof(uint64_t)). */
} httpServer_2014_2_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void httpServer_2014_2_run(
	max_engine_t *engine,
	httpServer_2014_2_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'default'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *httpServer_2014_2_run_nonblock(
	max_engine_t *engine,
	httpServer_2014_2_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void httpServer_2014_2_run_group(max_group_t *group, httpServer_2014_2_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *httpServer_2014_2_run_group_nonblock(max_group_t *group, httpServer_2014_2_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void httpServer_2014_2_run_array(max_engarray_t *engarray, httpServer_2014_2_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *httpServer_2014_2_run_array_nonblock(max_engarray_t *engarray, httpServer_2014_2_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* httpServer_2014_2_convert(max_file_t *maxfile, httpServer_2014_2_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* httpServer_2014_2_init(void);

/* Error handling functions */
int httpServer_2014_2_has_errors(void);
const char* httpServer_2014_2_get_errors(void);
void httpServer_2014_2_clear_errors(void);
/* Free statically allocated maxfile data */
void httpServer_2014_2_free(void);
/* These are dummy functions for hardware builds. */
int httpServer_2014_2_simulator_start(void);
int httpServer_2014_2_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_httpServer_2014_2_H */

