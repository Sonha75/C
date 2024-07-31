// This file is generated by ZCL Advanced Platform generator. Please don't edit manually.

// Enclosing macro to prevent multiple inclusion
#ifndef SILABS_PRINT_CLUSTER
#define SILABS_PRINT_CLUSTER


// This is the mapping of IDs to cluster names assuming a format according
// to the "EmberAfClusterName" defined in the ZCL header.
// The names of clusters that are not present, are removed.

  #define SILABS_PRINTCLUSTER_BASIC_CLUSTER { ZCL_BASIC_CLUSTER_ID, 0x0000, "Basic" },
  #define SILABS_PRINTCLUSTER_OTA_CONFIGURATION_CLUSTER { ZCL_OTA_CONFIGURATION_CLUSTER_ID, 0x1002, "Configuration Cluster" },
  #define SILABS_PRINTCLUSTER_HEIGHT_BIN { ZCL_HEIGHT_BIN_ID, 0x1011, "Height bin" },
  #define SILABS_PRINTCLUSTER_WEIGHT_BIN { ZCL_WEIGHT_BIN_ID, 0x1050, "Weight bin" },

#define CLUSTER_IDS_TO_NAMES \
  SILABS_PRINTCLUSTER_BASIC_CLUSTER \
  SILABS_PRINTCLUSTER_OTA_CONFIGURATION_CLUSTER \
  SILABS_PRINTCLUSTER_HEIGHT_BIN \
  SILABS_PRINTCLUSTER_WEIGHT_BIN \

#endif // SILABS_PRINT_CLUSTER
