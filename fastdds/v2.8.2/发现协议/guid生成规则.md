src/cpp/rtps/RTPSDomain.cpp

```cpp
    // Generate a new GuidPrefix_t
    GuidPrefix_t guidP;
    guid_prefix_create(ID, guidP);
    if (!PParam.builtin.metatraffic_external_unicast_locators.empty())
    {
        fastdds::rtps::LocatorList locators;
        fastrtps::rtps::IPFinder::getIP4Address(&locators);
        fastdds::rtps::ExternalLocatorsProcessor::add_external_locators(locators,
                PParam.builtin.metatraffic_external_unicast_locators);
        uint16_t host_id = Host::compute_id(locators);
        guidP.value[2] = static_cast<octet>(host_id & 0xFF);
        guidP.value[3] = static_cast<octet>((host_id >> 8) & 0xFF);
    }
```
