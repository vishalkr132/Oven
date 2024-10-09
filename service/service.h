#ifndef _SERVICE_H_
#define _SERVICE_H_

namespace Oven_project
{

    class Oven
    {
        public:
            /* Oven constructor */
            Oven();

            /* Oven destructor */
            ~Oven();

            static int SERVICE_fnReadOdor();
            static int SERVICE_fnReadTemp();
            static int SERVICE_fnReadRadar();
            static int SERVICE_fnSendDataToThingspeak();

        protected:

        private:
            static int SERVICE_fnInitOdor();
            static int SERVICE_fnInitTemp();
            static int SERVICE_fnInitRadar();
            static int SERVICE_fnInitTcp();

    }


}

#endif