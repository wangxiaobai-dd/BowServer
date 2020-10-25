#include <fstream>
#include <memory>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor_database.h>

using namespace google::protobuf;
using MessagePtr = std::shared_ptr<Message>;

class SelfProto
{
    public:
	static MessagePtr createMessage(const std::string& type_name)
	{
	    MessagePtr msgPtr;
	    google::protobuf::Message* message = nullptr;
	    const google::protobuf::Descriptor* descriptor =
		SelfProto::poolPtr->FindMessageTypeByName(type_name);
	    if (descriptor)
	    {
		const google::protobuf::Message* prototype =
		    SelfProto::factoryPtr->GetPrototype(descriptor);
		if (prototype)
		{
		    message = prototype->New();
		}
	    }
	    msgPtr.reset(message);
	    return msgPtr;
	}
	static bool openDesSet()
	{
	    std::ifstream desFileStream("protos/selfdes.set");
	    if(!desFileStream.is_open())
		return false;

	    FileDescriptorSet fds;
	    if(!fds.ParseFromIstream(&desFileStream))
		return false;

	    // SimpleDescriptorDatabase db;
	    for(int i = 0; i < fds.file_size(); ++i)
	    {
		poolPtr->BuildFile(fds.file(i));
		std::cout << fds.file(i).name() << std::endl;
	    }

            //poolPtr = std::make_shared<DescriptorPool>(&db);
	    factoryPtr = std::make_shared<DynamicMessageFactory>(poolPtr.get());
	    return true;
	}
	static std::shared_ptr<DescriptorPool> poolPtr;
	static std::shared_ptr<DynamicMessageFactory> factoryPtr;
};
