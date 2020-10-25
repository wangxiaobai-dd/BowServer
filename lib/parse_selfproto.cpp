#include "parse_selfproto.h"

using namespace google::protobuf;

std::shared_ptr<DescriptorPool> SelfProto::poolPtr = std::make_shared<DescriptorPool>();
std::shared_ptr<DynamicMessageFactory> SelfProto::factoryPtr = std::make_shared<DynamicMessageFactory>();

