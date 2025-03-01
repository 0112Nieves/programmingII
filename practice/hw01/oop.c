#include "oop.h"
void print_potato(const struct Potato *this){
    if(this == NULL) return;
    if(this){
        printf("Potato:\n");
        printf("    weight: %d\n", this->weight);
        printf("    produced by: %s\n", this->producer_name);
    }
}
void print_magical_potato(const struct Potato *this){
    if(this == NULL) return;
    struct MagicalPotato *magical_potato = (struct MagicalPotato*)malloc(sizeof(struct MagicalPotato));       
    memcpy(magical_potato, this,sizeof(struct MagicalPotato));
    printf("Magic Potato:\n");
    printf("    weight: %d\n", magical_potato->base.weight);
    printf("    produced by: %s\n", magical_potato->base.producer_name);
    printf("    magical effect: %s\n", MagicalEffectToString[magical_potato->effect]);
    printf("    magic level: %d\n", magical_potato->effect_level);
    free(magical_potato);
}
void potato_dtor(struct Potato **obj){
    if(obj == NULL || *obj == NULL) return;
    free(*obj);
    *obj = NULL;
}
void magical_potato_dtor(struct Potato **obj){
    if(obj == NULL || *obj == NULL) return;
    struct MagicalPotato **magical_obj = (struct MagicalPotato **)obj;
    free(*magical_obj);
    *magical_obj = NULL;
}
void print_potato_ball(const struct PotatoBall *this){
    if(this == NULL) return;
    printf("Potato Ball:\n");
    printf("    weight: %d\n", this->weight);
    printf("    produced by: %s\n", this->producer_name);
}
void print_magical_potato_ball(const struct PotatoBall *this){
    if(this == NULL) return;
    struct MagicalPotatoBall *magical_potato_ball = (struct MagicalPotatoBall*)malloc(sizeof(struct MagicalPotatoBall));   
    memcpy(magical_potato_ball,this, sizeof(struct MagicalPotatoBall));
    printf("Magic Potato Ball:\n");
    printf("    weight: %d\n", magical_potato_ball->base.weight);
    printf("    produced by: %s\n", magical_potato_ball->base.producer_name);
    printf("    magical effect: %s\n", MagicalEffectToString[magical_potato_ball->effect]);
    printf("    magic level: %d\n", magical_potato_ball->effect_level);
    free(magical_potato_ball);
}
void potato_ball_dtor(struct PotatoBall **obj){
    if(obj == NULL || *obj == NULL) return;
    free(*obj);
    *obj = NULL;
    return;
}
void magical_potato_ball_dtor(struct PotatoBall **obj){
    if(obj == NULL || *obj == NULL) return;
    struct MagicalPotatoBall **magical_ball_obj = (struct MagicalPotatoBall **)obj;
    free(*magical_ball_obj);
    *magical_ball_obj = NULL;
    return;
}
struct PotatoBall* fry_potato_ball(struct Potato **input){
    if(input == NULL || *input == NULL) return NULL;
    struct PotatoBall *potato_ball = (struct PotatoBall *)malloc(sizeof(*potato_ball));
    if(potato_ball == NULL) return NULL;
    potato_ball->weight = (*input)->weight;
    potato_ball->producer_name = (*input)->producer_name;
    potato_ball->dtor = &potato_ball_dtor;
    potato_ball->print = &print_potato_ball;
    free(*input);
    *input = NULL;
    return potato_ball;
}
struct PotatoBall* fry_magical_potato_ball(struct Potato **input){
    struct MagicalPotatoBall *magical_potato_ball = (struct MagicalPotatoBall*)malloc(sizeof(*magical_potato_ball));
    if(input == NULL || *input == NULL) return (struct PotatoBall *)magical_potato_ball;
    struct MagicalPotato *magical_potato = (struct MagicalPotato*)malloc(sizeof(struct MagicalPotato));  
    if(magical_potato_ball == NULL){
        return NULL;
    }
    memcpy(magical_potato, (*input), sizeof(struct MagicalPotato));
    if(magical_potato&&magical_potato_ball){
        magical_potato_ball->base.print = &print_magical_potato_ball;
        magical_potato_ball->base.dtor = &magical_potato_ball_dtor;
        magical_potato_ball->base.weight = magical_potato->base.weight;
        magical_potato_ball->base.producer_name = magical_potato->base.producer_name;
        magical_potato_ball->effect = magical_potato->effect;
        magical_potato_ball->effect_level = magical_potato->effect_level;
    }
    free(magical_potato);
    magical_potato_dtor(input);
    return (struct PotatoBall *)magical_potato_ball;
}
struct Potato *smallten_summon_potato(const struct PotatoProducer *this){
    if(this == NULL || this->name == NULL) return NULL;
    srand(1);
    init_smallten((struct PotatoProducer *)this);
    struct MagicalPotato *magical_potato = (struct MagicalPotato *)malloc(sizeof(struct MagicalPotato));
    if(magical_potato){
        magical_potato->base.print = &print_magical_potato;
        magical_potato->base.fry = &fry_magical_potato_ball;
        magical_potato->base.weight = rand();
        magical_potato->base.producer_name = this->name;
        magical_potato->effect = EFF_Haste;
        magical_potato->effect_level = 10;
    }
    return (struct Potato *)magical_potato;
}
struct Potato *subarya_produce_potato(const struct PotatoProducer *this){
    if(this == NULL || this->name == NULL) return NULL;
    srand(1);
    init_subarya((struct PotatoProducer *)this);
    struct Potato *potato = (struct Potato *)malloc(sizeof(struct Potato));
    if(potato){
        potato->print = &print_potato;
        potato->fry = &fry_potato_ball;
        potato->weight = rand();
        potato->producer_name = this->name;
    }
    return potato;
}
void init_smallten(struct PotatoProducer *obj){
    if(obj == NULL) return;
    obj->produce = &smallten_summon_potato;
    if(obj){
        strcpy(obj->name, "Smallten");
    }
}
void init_subarya(struct PotatoProducer *obj){
    if(obj == NULL) return;
    obj->produce = &subarya_produce_potato;
    if(obj){
        strcpy(obj->name, "Subarya");
    }
}
