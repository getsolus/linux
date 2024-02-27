#define PRD_SIZE_MASK            0xffffff
#define PRD_FLAG_END             0x1000000
#define PRD_FLAG_SUCCESS         0x2000000
#define PRD_FLAG_ERROR           0x4000000

struct ithc_phys_region_desc {
	u64 addr; // physical addr/1024
	u32 size; // num bytes, PRD_FLAG_END marks last prd for data split over multiple prds
	u32 unused;
};

#define DMA_RX_CODE_INPUT_REPORT          3
#define DMA_RX_CODE_FEATURE_REPORT        4
#define DMA_RX_CODE_REPORT_DESCRIPTOR     5
#define DMA_RX_CODE_RESET                 7

struct ithc_dma_rx_header {
	u32 code;
	u32 data_size;
	u32 _unknown[14];
};

#define DMA_TX_CODE_SET_FEATURE           3
#define DMA_TX_CODE_GET_FEATURE           4
#define DMA_TX_CODE_OUTPUT_REPORT         5
#define DMA_TX_CODE_GET_REPORT_DESCRIPTOR 7

struct ithc_dma_tx_header {
	u32 code;
	u32 data_size;
};

struct ithc_dma_prd_buffer {
	void *addr;
	dma_addr_t dma_addr;
	u32 size;
	u32 num_pages; // per data buffer
	enum dma_data_direction dir;
};

struct ithc_dma_data_buffer {
	void *addr;
	struct sg_table *sgt;
	int active_idx;
	u32 data_size;
};

struct ithc_dma_tx {
	struct mutex mutex;
	u32 max_size;
	struct ithc_dma_prd_buffer prds;
	struct ithc_dma_data_buffer buf;
};

struct ithc_dma_rx {
	struct mutex mutex;
	u32 num_received;
	struct ithc_dma_prd_buffer prds;
	struct ithc_dma_data_buffer bufs[NUM_RX_BUF];
};

int ithc_dma_rx_init(struct ithc *ithc, u8 channel, const char *devname);
void ithc_dma_rx_enable(struct ithc *ithc, u8 channel);
int ithc_dma_tx_init(struct ithc *ithc);
int ithc_dma_rx(struct ithc *ithc, u8 channel);
int ithc_dma_tx(struct ithc *ithc, u32 cmdcode, u32 datasize, void *cmddata);

