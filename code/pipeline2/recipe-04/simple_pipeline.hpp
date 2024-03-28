#pragma once

#include "pipeline.hpp"
#include "simple_data_source.hpp"
#include "simple_data_sink.hpp"
#include "simple_data_filter.hpp"

template <typename SourceDataType, typename SinkDataType>
class SimplePipeline: public Pipeline<SourceDataType, SinkDataType> {
public:
    using Base = Pipeline<SourceDataType, SinkDataType>;

    using Base::addDataSource;
    using Base::addDataFilter;
    using Base::addDataSink;

    SimplePipeline(): SimplePipeline(make_pipe<SourceDataType>()) {
    }

    SimplePipeline(Pipe<SourceDataType> source_pipe_)
        : Pipeline<SourceDataType, SinkDataType>(source_pipe_) {
    }

    ~SimplePipeline() = default;

    SimplePipeline& addDataSource(std::function<bool(SourceDataType&)> func) {
        std::shared_ptr<DataSource<SourceDataType>> data_source{
            new SimpleDataSource<SourceDataType>(func)
        };
        this->Base::addDataSource(data_source);
        return *this;
    }

    SimplePipeline& addDataSink(std::function<void(SinkDataType&)> func) {
        std::shared_ptr<DataSink<SinkDataType>> data_sink{
            new SimpleDataSink<SinkDataType>(func)
        };
        this->Base::addDataSink(data_sink);
        return *this;
    }

    template <typename IT, typename OT>
    SimplePipeline& addDataFilter(std::function<OT(IT)> func) {
        std::shared_ptr<DataFilterAny> data_filter{
            new SimpleDataFilter<IT, OT>(func)
        };
        auto next_pipe = make_pipe<OT>();
        this->Base::addDataFilterAny(data_filter, next_pipe);
        return *this;
    }
};
