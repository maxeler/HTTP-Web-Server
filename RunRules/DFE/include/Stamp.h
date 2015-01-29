/**\file */
#ifndef SLIC_DECLARATIONS_Stamp_H
#define SLIC_DECLARATIONS_Stamp_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define Stamp_USE_NETWORK_MANAGER (1)


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
 * \param [out] outscalar_TxManagerKernelV1_kernel_frameCount Output scalar parameter "TxManagerKernelV1_kernel.frameCount".
 * \param [in] instream_fromCpu Stream "fromCpu".
 * \param [in] instream_size_fromCpu The size of the stream instream_fromCpu in bytes.
 * \param [in] instream_network_mgmt_stream_from_host_tx_framed Stream "network_mgmt_stream_from_host_tx_framed".
 * \param [in] instream_size_network_mgmt_stream_from_host_tx_framed The size of the stream instream_network_mgmt_stream_from_host_tx_framed in bytes.
 * \param [out] outstream_network_mgmt_stream_to_host_rx_framed Stream "network_mgmt_stream_to_host_rx_framed".
 * \param [in] outstream_size_network_mgmt_stream_to_host_rx_framed The size of the stream outstream_network_mgmt_stream_to_host_rx_framed in bytes.
 * \param [out] outstream_toCpu Stream "toCpu".
 * \param [in] outstream_size_toCpu The size of the stream outstream_toCpu in bytes.
 * \param [out] outstream_toCpuByteNumber Stream "toCpuByteNumber".
 * \param [in] outstream_size_toCpuByteNumber The size of the stream outstream_toCpuByteNumber in bytes.
 * \param [out] outstream_toCpuOutputTxEvent Stream "toCpuOutputTxEvent".
 * \param [in] outstream_size_toCpuOutputTxEvent The size of the stream outstream_toCpuOutputTxEvent in bytes.
 * \param [in] inmem_stamp_state_machine_romHashIndex Mapped ROM inmem_stamp_state_machine_romHashIndex, should be of size (32768 * sizeof(uint64_t)).
 * \param [in] inmem_stamp_state_machine_romMapped Mapped ROM inmem_stamp_state_machine_romMapped, should be of size (32768 * sizeof(uint64_t)).
 */
void Stamp(
	uint64_t ticks_KernelLMem_Read_CommandAndDataStream,
	uint64_t ticks_KernelLMem_Write_CommandAndDataStream,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst,
	uint64_t *outscalar_TxManagerKernelV1_kernel_frameCount,
	const void *instream_fromCpu,
	size_t instream_size_fromCpu,
	const void *instream_network_mgmt_stream_from_host_tx_framed,
	size_t instream_size_network_mgmt_stream_from_host_tx_framed,
	void *outstream_network_mgmt_stream_to_host_rx_framed,
	size_t outstream_size_network_mgmt_stream_to_host_rx_framed,
	void *outstream_toCpu,
	size_t outstream_size_toCpu,
	void *outstream_toCpuByteNumber,
	size_t outstream_size_toCpuByteNumber,
	void *outstream_toCpuOutputTxEvent,
	size_t outstream_size_toCpuOutputTxEvent,
	const uint64_t *inmem_stamp_state_machine_romHashIndex,
	const uint64_t *inmem_stamp_state_machine_romMapped);

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
 * \param [out] outscalar_TxManagerKernelV1_kernel_frameCount Output scalar parameter "TxManagerKernelV1_kernel.frameCount".
 * \param [in] instream_fromCpu Stream "fromCpu".
 * \param [in] instream_size_fromCpu The size of the stream instream_fromCpu in bytes.
 * \param [in] instream_network_mgmt_stream_from_host_tx_framed Stream "network_mgmt_stream_from_host_tx_framed".
 * \param [in] instream_size_network_mgmt_stream_from_host_tx_framed The size of the stream instream_network_mgmt_stream_from_host_tx_framed in bytes.
 * \param [out] outstream_network_mgmt_stream_to_host_rx_framed Stream "network_mgmt_stream_to_host_rx_framed".
 * \param [in] outstream_size_network_mgmt_stream_to_host_rx_framed The size of the stream outstream_network_mgmt_stream_to_host_rx_framed in bytes.
 * \param [out] outstream_toCpu Stream "toCpu".
 * \param [in] outstream_size_toCpu The size of the stream outstream_toCpu in bytes.
 * \param [out] outstream_toCpuByteNumber Stream "toCpuByteNumber".
 * \param [in] outstream_size_toCpuByteNumber The size of the stream outstream_toCpuByteNumber in bytes.
 * \param [out] outstream_toCpuOutputTxEvent Stream "toCpuOutputTxEvent".
 * \param [in] outstream_size_toCpuOutputTxEvent The size of the stream outstream_toCpuOutputTxEvent in bytes.
 * \param [in] inmem_stamp_state_machine_romHashIndex Mapped ROM inmem_stamp_state_machine_romHashIndex, should be of size (32768 * sizeof(uint64_t)).
 * \param [in] inmem_stamp_state_machine_romMapped Mapped ROM inmem_stamp_state_machine_romMapped, should be of size (32768 * sizeof(uint64_t)).
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *Stamp_nonblock(
	uint64_t ticks_KernelLMem_Read_CommandAndDataStream,
	uint64_t ticks_KernelLMem_Write_CommandAndDataStream,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Read_CommandAndDataStream_wordsPerBurst,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_totalBursts,
	uint64_t inscalar_KernelLMem_Write_CommandAndDataStream_wordsPerBurst,
	uint64_t *outscalar_TxManagerKernelV1_kernel_frameCount,
	const void *instream_fromCpu,
	size_t instream_size_fromCpu,
	const void *instream_network_mgmt_stream_from_host_tx_framed,
	size_t instream_size_network_mgmt_stream_from_host_tx_framed,
	void *outstream_network_mgmt_stream_to_host_rx_framed,
	size_t outstream_size_network_mgmt_stream_to_host_rx_framed,
	void *outstream_toCpu,
	size_t outstream_size_toCpu,
	void *outstream_toCpuByteNumber,
	size_t outstream_size_toCpuByteNumber,
	void *outstream_toCpuOutputTxEvent,
	size_t outstream_size_toCpuOutputTxEvent,
	const uint64_t *inmem_stamp_state_machine_romHashIndex,
	const uint64_t *inmem_stamp_state_machine_romMapped);

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
	uint64_t *outscalar_TxManagerKernelV1_kernel_frameCount; /**<  [out] Output scalar parameter "TxManagerKernelV1_kernel.frameCount". */
	const void *instream_fromCpu; /**<  [in] Stream "fromCpu". */
	size_t instream_size_fromCpu; /**<  [in] The size of the stream instream_fromCpu in bytes. */
	const void *instream_network_mgmt_stream_from_host_tx_framed; /**<  [in] Stream "network_mgmt_stream_from_host_tx_framed". */
	size_t instream_size_network_mgmt_stream_from_host_tx_framed; /**<  [in] The size of the stream instream_network_mgmt_stream_from_host_tx_framed in bytes. */
	void *outstream_network_mgmt_stream_to_host_rx_framed; /**<  [out] Stream "network_mgmt_stream_to_host_rx_framed". */
	size_t outstream_size_network_mgmt_stream_to_host_rx_framed; /**<  [in] The size of the stream outstream_network_mgmt_stream_to_host_rx_framed in bytes. */
	void *outstream_toCpu; /**<  [out] Stream "toCpu". */
	size_t outstream_size_toCpu; /**<  [in] The size of the stream outstream_toCpu in bytes. */
	void *outstream_toCpuByteNumber; /**<  [out] Stream "toCpuByteNumber". */
	size_t outstream_size_toCpuByteNumber; /**<  [in] The size of the stream outstream_toCpuByteNumber in bytes. */
	void *outstream_toCpuOutputTxEvent; /**<  [out] Stream "toCpuOutputTxEvent". */
	size_t outstream_size_toCpuOutputTxEvent; /**<  [in] The size of the stream outstream_toCpuOutputTxEvent in bytes. */
	const uint64_t *inmem_stamp_state_machine_romHashIndex; /**<  [in] Mapped ROM inmem_stamp_state_machine_romHashIndex, should be of size (32768 * sizeof(uint64_t)). */
	const uint64_t *inmem_stamp_state_machine_romMapped; /**<  [in] Mapped ROM inmem_stamp_state_machine_romMapped, should be of size (32768 * sizeof(uint64_t)). */
} Stamp_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void Stamp_run(
	max_engine_t *engine,
	Stamp_actions_t *interface_actions);

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
max_run_t *Stamp_run_nonblock(
	max_engine_t *engine,
	Stamp_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void Stamp_run_group(max_group_t *group, Stamp_actions_t *interface_actions);

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
max_run_t *Stamp_run_group_nonblock(max_group_t *group, Stamp_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void Stamp_run_array(max_engarray_t *engarray, Stamp_actions_t *interface_actions[]);

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
max_run_t *Stamp_run_array_nonblock(max_engarray_t *engarray, Stamp_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* Stamp_convert(max_file_t *maxfile, Stamp_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* Stamp_init(void);

/* Error handling functions */
int Stamp_has_errors(void);
const char* Stamp_get_errors(void);
void Stamp_clear_errors(void);
/* Free statically allocated maxfile data */
void Stamp_free(void);
/* These are dummy functions for hardware builds. */
int Stamp_simulator_start(void);
int Stamp_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_Stamp_H */

