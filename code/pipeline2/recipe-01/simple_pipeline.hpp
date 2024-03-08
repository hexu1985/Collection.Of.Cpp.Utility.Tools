#pragma once

#include "pipeline.hpp"
#include "simple_data_source.hpp"
#include "simple_data_sink.hpp"
#include "simple_data_filter.hpp"

template <typename SourceDataType, typename SinkDataType>
class SimplePipeline: public Pipeline<SourceDataType, SinkDataType> {
public:
    using Base = Pipeline<SourceDataType, SinkDataType>;

    SimplePipeline() = default;
    ~SimplePipeline() = default;

    SimplePipeline& addDataSource(std::function<bool(SourceDataType&)> func) {
        std::shared_ptr<DataSource<SourceDataType>> data_source{
            new SimpleDataSource<SourceDataType>(func)
        };
        this->Base::addDataSource(data_source);
        return *this;
    }

    SimplePipeline& addDataSink(std::function<void(SinkDataType&)> func) {
        std::shared_ptr<DataSink<SourceDataType>> data_sink{
            new SimpleDataSource<SourceDataType>(func)
        };
        this->Base::addDataSink(data_sink);
        return *this;
    }

    template <typename IT, typename OT>
    SimplePipeline& addDataFilter(std::function<OT(IT)> func) {
        std::shared_ptr<DataFilter<IT, OT>> data_filter{
            new SimpleDataFilter<IT, OT>(func)
        };
        this->Base::addDataFilter(data_filter);
        return *this;
    }
};
