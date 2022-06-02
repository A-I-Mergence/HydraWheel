#include "mbed.h"
#include "HydraWheel.h"
#include "Wheel.h"


myHydraWheel::myHydraWheel():HydraComponent("myHydraWheel", "myHydraWheel/type")
{
    _bus = new BusOut(PB_7, PB_6, PB_5, PB_4);
    _wheel = new Wheel(D3, D9, D6, D5, D4);       //pwm, fwd, rev, EncA, EncB
    _wheel->StartRegule();
    // // Timer
    // _t = new Timer;
    // _t->start();
    _des_spd  = 0;

    // Timer
    _t = new Timer;
    _t->start();

    _modeSave = true;
    _FlagNewCommande = true;

    addHydraFunction(new HydraFunction("move",        callback(this, &myHydraWheel::move),         1, 10));
    addHydraFunction(new HydraFunction("stop",        callback(this, &myHydraWheel::stop),         0, 11));
    addHydraFunction(new HydraFunction("save",        callback(this, &myHydraWheel::setSaveMode),  1, 12));
    addHydraFunction(new HydraFunction("pwm",         callback(this, &myHydraWheel::setPwm),       1, 14));
    addHydraFunction(new HydraFunction("setChannel",  callback(this, &myHydraWheel::setChannel),   1));


    // addHydraVariable(new HydraVariable("TOTO", new HydraData(&_toto), 12));
}

void myHydraWheel::__loop__()
{
    if (_modeSave) {
        // printf("time : %f \n",_t->read());
        if (_t->read_ms() > 50){
            // print("timeloop \n");
            if (!_FlagNewCommande){
                // print("flag \n");
                _wheel->StartRegule();
                _wheel->SetSpeed(0);
            }
            _t->reset();
            _t->start();
        }
    }
}

void myHydraWheel::__on__(std::string event_name, std::map<std::string, HydraData *> data){}

// Component function
HydraData* myHydraWheel::move(std::vector<HydraData*> parameters)
{
    _FlagNewCommande = false; 
    int param_1 = parameters[0]->get<int>();
    _wheel->StartRegule();
    _wheel->SetSpeed(param_1);
    return new HydraData("vitesse: " +to_string(param_1));
}

HydraData* myHydraWheel::stop(std::vector<HydraData*> parameters){
    _wheel->StartRegule();
    _des_spd = 0;
    _wheel->SetSpeed(_des_spd);
    return NULL;
}

HydraData* myHydraWheel::setSaveMode(std::vector<HydraData*> parameters){
    _modeSave = parameters[0]->get<int>() != 0 ? true : false;
    // _wheel->SetPWM(0);
    return NULL;
}

HydraData* myHydraWheel::setPwm(std::vector<HydraData*> parameters){
    float _pwm = parameters[0]->get<float>() > 1 ? 1 : parameters[0]->get<float>() < -1 ? -1 : parameters[0]->get<float>();
    _wheel->StopRegule();
    _wheel->SetPWM(_pwm);
    return NULL;
}

HydraData* myHydraWheel::setChannel(std::vector<HydraData*> parameters){
    send_CAN_command(99, parameters);
    return NULL;
}