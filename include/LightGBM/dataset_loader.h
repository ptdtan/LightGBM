#ifndef LIGHTGBM_DATASET_LOADER_H_
#define LIGHTGBM_DATASET_LOADER_H_

#include <LightGBM/dataset.h>

namespace LightGBM {

class DatasetLoader {
 public:
  LIGHTGBM_EXPORT DatasetLoader(const Config& io_config, const PredictFunction& predict_fun, int num_class, const char* filename);

  LIGHTGBM_EXPORT ~DatasetLoader();

  LIGHTGBM_EXPORT Dataset* LoadFromFile(const char* filename, const char* initscore_file, int rank, int num_machines);

  LIGHTGBM_EXPORT Dataset* LoadFromFile(const char* filename, const char* initscore_file) {
    return LoadFromFile(filename, initscore_file, 0, 1);
  }

  LIGHTGBM_EXPORT Dataset* LoadFromFileAlignWithOtherDataset(const char* filename, const char* initscore_file, const Dataset* train_data);

  LIGHTGBM_EXPORT Dataset* CostructFromSampleData(double** sample_values,
    int** sample_indices, int num_col, const int* num_per_col,
    size_t total_sample_size, data_size_t num_data);

  /*! \brief Disable copy */
  DatasetLoader& operator=(const DatasetLoader&) = delete;
  /*! \brief Disable copy */
  DatasetLoader(const DatasetLoader&) = delete;

 private:
  Dataset* LoadFromBinFile(const char* data_filename, const char* bin_filename, int rank, int num_machines, int* num_global_data, std::vector<data_size_t>* used_data_indices);

  void SetHeader(const char* filename);

  void CheckDataset(const Dataset* dataset);

  std::vector<std::string> LoadTextDataToMemory(const char* filename, const Metadata& metadata, int rank, int num_machines, int* num_global_data, std::vector<data_size_t>* used_data_indices);

  std::vector<std::string> SampleTextDataFromMemory(const std::vector<std::string>& data);

  std::vector<std::string> SampleTextDataFromFile(const char* filename, const Metadata& metadata, int rank, int num_machines, int* num_global_data, std::vector<data_size_t>* used_data_indices);

  void ConstructBinMappersFromTextData(int rank, int num_machines, const std::vector<std::string>& sample_data, const Parser* parser, Dataset* dataset);

  /*! \brief Extract local features from memory */
  void ExtractFeaturesFromMemory(std::vector<std::string>& text_data, const Parser* parser, Dataset* dataset);

  /*! \brief Extract local features from file */
  void ExtractFeaturesFromFile(const char* filename, const Parser* parser, const std::vector<data_size_t>& used_data_indices, Dataset* dataset);

  /*! \brief Check can load from binary file */
  std::string CheckCanLoadFromBin(const char* filename);

  const Config& config_;
  /*! \brief Random generator*/
  Random random_;
  /*! \brief prediction function for initial model */
  const PredictFunction& predict_fun_;
  /*! \brief number of classes */
  int num_class_;
  /*! \brief index of label column */
  int label_idx_;
  /*! \brief index of weight column */
  int weight_idx_;
  /*! \brief index of group column */
  int group_idx_;
  /*! \brief Mapper from real feature index to used index*/
  std::unordered_set<int> ignore_features_;
  /*! \brief store feature names */
  std::vector<std::string> feature_names_;
  /*! \brief Mapper from real feature index to used index*/
  std::unordered_set<int> categorical_features_;
};

}

#endif  // LIGHTGBM_DATASET_LOADER_H_
