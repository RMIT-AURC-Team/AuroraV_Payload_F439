var group__DMA__Exported__Types =
[
    [ "DMA_InitTypeDef", "group__DMA__Exported__Types.html#structDMA__InitTypeDef", [
      [ "Channel", "group__DMA__Exported__Types.html#ae82bf9242a014164f9f6907f29782c44", null ],
      [ "Direction", "group__DMA__Exported__Types.html#ab94410c1333b512e271b1c135fe50916", null ],
      [ "PeriphInc", "group__DMA__Exported__Types.html#a4925ca3ceb52340daddc92817dc304d9", null ],
      [ "MemInc", "group__DMA__Exported__Types.html#a831756fbcd64feb1e570a9bf743b5b8d", null ],
      [ "PeriphDataAlignment", "group__DMA__Exported__Types.html#aca5b89241171c093fd0fc6dacf72683c", null ],
      [ "MemDataAlignment", "group__DMA__Exported__Types.html#afe3adac32f5411b1a744c030f398aa5e", null ],
      [ "Mode", "group__DMA__Exported__Types.html#a0ffc93ec511ed9cf1663f6939bd3e839", null ],
      [ "Priority", "group__DMA__Exported__Types.html#a72acf77c0b19359eb70764505ae4bd70", null ],
      [ "FIFOMode", "group__DMA__Exported__Types.html#a8dc149e98014264da61675f6a0e18b88", null ],
      [ "FIFOThreshold", "group__DMA__Exported__Types.html#a28732ef5d9eae23dbd77e3034cc1bdb3", null ],
      [ "MemBurst", "group__DMA__Exported__Types.html#a4e6e9f06e5c7903879ed29df299e4df1", null ],
      [ "PeriphBurst", "group__DMA__Exported__Types.html#ae95b14383c9d0c86405939f4bfe2882d", null ]
    ] ],
    [ "DMA_HandleTypeDef", "group__DMA__Exported__Types.html#structDMA__HandleTypeDef", [
      [ "Instance", "group__DMA__Exported__Types.html#acf2416c7f97b4f21ef16cfa5bea3cfd3", null ],
      [ "Init", "group__DMA__Exported__Types.html#a4352c7144ad5e1e4ab54a87d3be6eb62", null ],
      [ "Lock", "group__DMA__Exported__Types.html#ad4cf225029dbefe8d3fe660c33b8bb6b", null ],
      [ "State", "group__DMA__Exported__Types.html#a2263083d2bfa96222f3d7b8339c6faf8", null ],
      [ "Parent", "group__DMA__Exported__Types.html#a6ee5f2130887847bbc051932ea43b73d", null ],
      [ "XferCpltCallback", "group__DMA__Exported__Types.html#a3d7716d1d7a5717f09525efd19334864", null ],
      [ "XferHalfCpltCallback", "group__DMA__Exported__Types.html#aea6af14bab20d3f9a82f08df1abea01a", null ],
      [ "XferM1CpltCallback", "group__DMA__Exported__Types.html#a7055720d00fe66e27f3910087cb6524a", null ],
      [ "XferM1HalfCpltCallback", "group__DMA__Exported__Types.html#a5f01e4cd22742ea0ae056adc50b68ed3", null ],
      [ "XferErrorCallback", "group__DMA__Exported__Types.html#a695c6fe664a7baf827e461652ebdb9e0", null ],
      [ "XferAbortCallback", "group__DMA__Exported__Types.html#a6253dc8680e566bbc244228374dd647d", null ],
      [ "ErrorCode", "group__DMA__Exported__Types.html#a123c5063e6a3b1901b2fbe5f88c53a7e", null ],
      [ "StreamBaseAddress", "group__DMA__Exported__Types.html#a053ff68722cdf5ac37aa305e04e2b1c8", null ],
      [ "StreamIndex", "group__DMA__Exported__Types.html#aa3ca4a9a3d300e05d7ff46613f43fd54", null ]
    ] ],
    [ "HAL_DMA_StateTypeDef", "group__DMA__Exported__Types.html#ga9c012af359987a240826f29073bbe463", [
      [ "HAL_DMA_STATE_RESET", "group__DMA__Exported__Types.html#gga9c012af359987a240826f29073bbe463a9e7be73da32b8c837cde0318e0d5eed2", null ],
      [ "HAL_DMA_STATE_READY", "group__DMA__Exported__Types.html#gga9c012af359987a240826f29073bbe463ad497944e6e72bc3ca904694b1098105a", null ],
      [ "HAL_DMA_STATE_BUSY", "group__DMA__Exported__Types.html#gga9c012af359987a240826f29073bbe463af7a0a2ca8de4e5be9e85b6a9073476ef", null ],
      [ "HAL_DMA_STATE_TIMEOUT", "group__DMA__Exported__Types.html#gga9c012af359987a240826f29073bbe463acf3a5443bf4dc71018512a255e2076eb", null ],
      [ "HAL_DMA_STATE_ERROR", "group__DMA__Exported__Types.html#gga9c012af359987a240826f29073bbe463ac2ce65c7cb2410c143b14e309ba83742", null ],
      [ "HAL_DMA_STATE_ABORT", "group__DMA__Exported__Types.html#gga9c012af359987a240826f29073bbe463af199cdb868cfd96fa97decb285643755", null ]
    ] ],
    [ "HAL_DMA_LevelCompleteTypeDef", "group__DMA__Exported__Types.html#gaee3245eea8fa938edeb35a6c9596fd86", [
      [ "HAL_DMA_FULL_TRANSFER", "group__DMA__Exported__Types.html#ggaee3245eea8fa938edeb35a6c9596fd86a5314147c8ba21548763bf89446b78468", null ],
      [ "HAL_DMA_HALF_TRANSFER", "group__DMA__Exported__Types.html#ggaee3245eea8fa938edeb35a6c9596fd86ad0ba8bc74a2ae6dcdc3e316e8be0d5d8", null ]
    ] ],
    [ "HAL_DMA_CallbackIDTypeDef", "group__DMA__Exported__Types.html#gafbe8b2bd9ce2128de6cdc08ccde7e8ad", [
      [ "HAL_DMA_XFER_CPLT_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8ada7d4463d9db2e6d15282128b44ae08e12", null ],
      [ "HAL_DMA_XFER_HALFCPLT_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8ada4b1606f39a4eec41d958bc878719f046", null ],
      [ "HAL_DMA_XFER_M1CPLT_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8ada09feb1bab1c32b35afd27b9316958051", null ],
      [ "HAL_DMA_XFER_M1HALFCPLT_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8adac2e68a660d9830fa1e965482b9befbb9", null ],
      [ "HAL_DMA_XFER_ERROR_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8ada3e76bc89154e0b50333cc551bf0337a6", null ],
      [ "HAL_DMA_XFER_ABORT_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8ada3059a9412e0624699e9123ba2bccdf3e", null ],
      [ "HAL_DMA_XFER_ALL_CB_ID", "group__DMA__Exported__Types.html#ggafbe8b2bd9ce2128de6cdc08ccde7e8adac9935fd906719942d6b09cfd55e837f0", null ]
    ] ]
];