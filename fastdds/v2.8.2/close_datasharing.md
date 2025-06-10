bool RTPSWriter::is_datasharing_compatible() const
{
    return (m_att.data_sharing_configuration().kind() != OFF);
}
